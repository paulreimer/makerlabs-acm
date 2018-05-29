#include "app_task.h"

#include <chrono>
#include <memory>
#include <string>

#include "actors.h"

#include "acm_helpers.h"
#include "actor_model.h"
#include "delay.h"
#include "firmware_update.h"
#include "firmware_update_actor.h"
#include "network.h"
#include "request_manager_actor.h"
#include "requests.h"
#include "spreadsheet_insert_row_actor.h"
#include "visualization_query_actor.h"

#include "display_generated.h"

#include "embedded_files.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "trace.h"

using namespace ActorModel;
// ActorModel behaviours:
using Requests::request_manager_behaviour;
using FirmwareUpdate::firmware_update_behaviour;
using FirmwareUpdate::get_current_firmware_version;
using googleapis::Visualization::visualization_query_actor_behaviour;

using string_view = std::experimental::string_view;
using string = std::string;

using Timestamp = std::chrono::time_point<std::chrono::system_clock>;

using namespace Display;
using namespace googleapis::Sheets;

using namespace std::chrono_literals;

constexpr char TAG[] = "app_task";

auto app_task(void* /* user_data */)
  -> void
{
  printf("S/W Version Number: %lld\n", get_current_firmware_version());
  string machine_id_str = CONFIG_PERMISSION_COLUMN_LABEL;
/*
  // Check boot state of GPIO5 (pull-down -> reset to factory immediately)
  uint32_t strapping_pins = REG_READ(GPIO_STRAP_REG);
  bool factory_reset_requested = ((strapping_pins & 1) == 0);
  if (factory_reset_requested)
  {
    // Check if we are already running from factory partition
    auto current_partition = get_current_partition();
    auto factory_partition = get_factory_partition();

    if (not compare_partitions(current_partition, factory_partition))
    {
      ESP_LOGW(TAG, "Trigger factory reset");
      factory_reset();
    }
    else {
      ESP_LOGW(TAG, "Factory reset request ignored, already running factory!");
    }
  }
*/
  heap_check("app_task");

  // DisplayActor
  {
    auto display_actor_pid = spawn(display_actor_behaviour);
    register_name("display", display_actor_pid);

    // Show version details on OLED display
    flatbuffers::FlatBufferBuilder fbb;

    auto version_str = string(
      "v" + std::to_string(get_current_firmware_version())
    );

    auto display_loc = CreateShowUserDetails(
      fbb,
      fbb.CreateString("MakerLabs ACM"),
      fbb.CreateString(version_str),
      fbb.CreateString(version_str),
      fbb.CreateString(version_str)
    );

    fbb.Finish(
      CreateDisplayIntent(
        fbb,
        DisplayAction::ShowUserDetails,
        display_loc.Union()
      ),
      DisplayIntentIdentifier()
    );

    send(
      display_actor_pid,
      "ShowUserDetails",
      fbb.Release()
    );
  }

  // Wait for valid network connection before making the connection
  wait_for_network(
    (NETWORK_IS_CONNECTED | NETWORK_TIME_AVAILABLE),
    portMAX_DELAY
  );
  ESP_LOGI(TAG, "Network online");

  // Spawn the RequestManager actor
  Pid request_manager_actor_pid;
  {
    // Spawn the RequestManager actor using the generated execution config
    request_manager_actor_pid = spawn(
      request_manager_behaviour,
      // Override the default execution config settings
      [](ActorExecutionConfigBuilder& builder)
      {
        builder.add_task_stack_size(REQUESTS_REQUEST_MANAGER_TASK_STACK_SIZE);
        builder.add_mailbox_size(REQUESTS_REQUEST_MANAGER_MAILBOX_SIZE);
      }
    );

    register_name("request_manager", request_manager_actor_pid);
  }

  // FirmwareUpdateActor
  {
    auto firmware_update_actor_pid = spawn(
      firmware_update_behaviour,
      // Override the default execution config settings
      [](ActorExecutionConfigBuilder& builder)
      {
        builder.add_task_stack_size(FIRMWARE_UPDATE_ACTOR_TASK_STACK_SIZE);
        builder.add_mailbox_size(FIRMWARE_UPDATE_ACTOR_MAILBOX_SIZE);
      }
    );
    register_name("firmware_update", firmware_update_actor_pid);
  }

  // {Auth, Sheets, Visualization, PermissionsCheck}Actor
  {
    auto combined_actor_pid = spawn(
      {
        auth_actor_behaviour,
        spreadsheet_insert_row_actor_behaviour,
        visualization_query_actor_behaviour,
      },
      // Override the default execution config settings to increase mailbox size
      [](ActorExecutionConfigBuilder& builder)
      {
        builder.add_task_stack_size(4096);
        builder.add_mailbox_size(8192);
      }
    );
    register_name("reauth", combined_actor_pid);
    register_name("sheets", combined_actor_pid);
    register_name("gviz", combined_actor_pid);
  }

  // RFIDReaderActor
  {
    auto rfid_reader_actor_pid = spawn(rfid_reader_actor_behaviour);
    register_name("rfid_reader", rfid_reader_actor_pid);
  }

  // AppActor
  {
    auto app_actor_pid = spawn(
      app_actor_behaviour,
      // Override the default execution config settings to increase mailbox size
      [](ActorExecutionConfigBuilder& builder)
      {
        builder.add_task_stack_size(4096);
      }
    );
    register_name("app", app_actor_pid);
  }

  // Set CA certs for *.google.com
  send(
    request_manager_actor_pid,
    "add_cacert_der",
    embedded_files::WILDCARD_google_com_root_cacert_der
  );

  // Set CA certs for *.googleapis.com
  send(
    request_manager_actor_pid,
    "add_cacert_der",
    embedded_files::WILDCARD_googleapis_com_root_cacert_der
  );

  // Main loop:
  auto firmware_update_check_interval = std::chrono::seconds(
    CONFIG_FIRMWARE_UPDATE_CHECK_INTERVAL_SECONDS
  );

  auto reauth_interval = 20min;
  auto network_check_interval = 10min;
  auto rfid_scan_interval = 100ms;
  auto reset_button_check_interval = 1s;

  Timestamp last_reset_button_check_timestamp;
  Timestamp last_firmware_update_check_timestamp;
  Timestamp last_reauth_timestamp;
  Timestamp last_network_check_timestamp;
  Timestamp last_rfid_scan_timestamp;
  Timestamp last_reset_button_trigger_timestamp;

  auto reset_button_pin = static_cast<gpio_num_t>(0);
  gpio_set_direction(reset_button_pin, GPIO_MODE_INPUT);
  auto sent_firmware_request_payload = false;

  uint32_t progress = 0;
  auto reset_count = 0;

  for (;;)
  {
    //heap_check("app_task loop");

    // Periodic polling loop, send interval-based messages here
    auto now = std::chrono::system_clock::now();

    // Progress reset count, eventually trigger a factory reset after 5s
    if ((now - last_reset_button_check_timestamp) > reset_button_check_interval)
    {
      // Invert logic on reset button (pull-down for 5s triggers factory reset)
      auto reset_button_pressed = (gpio_get_level(reset_button_pin) == 0);
      if (reset_button_pressed)
      {
        // Update progress bar by 20%
        reset_count++;
        auto progress_bar = generate_progress_bar("Reset...", reset_count * 20);
        auto display_actor_pid = *(whereis("display"));
        send(display_actor_pid, "ProgressBar", progress_bar);

        auto firmware_update_actor_pid = *(whereis("firmware_update"));
        send(firmware_update_actor_pid, "reset_pressed");
      }
      else {
        reset_count = 0;
      }

      last_reset_button_check_timestamp = now;
    }

    // Trigger firmware update check if interval has elapsed
    if ((now - last_firmware_update_check_timestamp) > firmware_update_check_interval)
    {
      auto firmware_update_actor_pid = *(whereis("firmware_update"));
      if (sent_firmware_request_payload)
      {
        // Trigger a firmware update check with a previously configured request intent
        send(firmware_update_actor_pid, "check");
      }
      else {
        // Send the full firmware update check request intent
        send(
          firmware_update_actor_pid,
          "check",
          embedded_files::firmware_update_check_request_intent_req_fb
        );
      }

      last_firmware_update_check_timestamp = now;
    }

    // Trigger reauth if interval has elapsed
    if ((now - last_reauth_timestamp) > reauth_interval)
    {
      // Send the request intent message to the request manager actor
      auto reauth_actor_pid = *(whereis("reauth"));
      send(reauth_actor_pid, "reauth");

      last_reauth_timestamp = now;
    }

    // Trigger network check if interval has elapsed
    if ((now - last_network_check_timestamp) > network_check_interval)
    {
      printf("trigger network check here\n");
      // TODO: trigger network check

      last_network_check_timestamp = now;
    }

    // Only begin scan activity after initial setup is done
    //if (ready_to_run)
    {
      if ((now - last_rfid_scan_timestamp) > rfid_scan_interval)
      {
        auto rfid_reader_actor_pid = *(whereis("rfid_reader"));
        send(rfid_reader_actor_pid, "scan");

        last_rfid_scan_timestamp = now;
      }
    }

    // Run the loop at approx. half the shortest interval
    delay(50ms);
  }

  ESP_LOGI(TAG, "Complete, deleting task.");
  vTaskDelete(nullptr);
}
