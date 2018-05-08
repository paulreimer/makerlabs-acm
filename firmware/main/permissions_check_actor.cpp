#include "actors.h"

#include "requests.h"

#include "acm_helpers.h"

#include "googleapis.h"
#include "gviz_generated.h"

#include "embedded_files.h"

#include "esp_log.h"

#include <string>
#include <experimental/string_view>

using namespace ActorModel;
using namespace Requests;

using namespace ACM;
using namespace GViz;
using namespace googleapis;

using string = std::string;
using string_view = std::experimental::string_view;

using UserFlatbuffer = flatbuffers::DetachedBuffer;

constexpr char TAG[] = "permissions_check_actor";

struct PermissionsCheckActorState
{
  bool ready_to_run = false;
  MutableRequestIntentFlatbuffer permissions_check_query_mutable_buf;
  GViz::Query* permissions_check_query = nullptr;
  UserFlatbuffer current_user_flatbuf;
  MutableRequestIntentFlatbuffer permissions_check_request_intent_mutable_buf;
  RequestIntent* permissions_check_request_intent = nullptr;
  MutableRequestIntentFlatbuffer activity_request_intent_mutable_buf;
  RequestIntent* activity_request_intent = nullptr;
  string machine_id_str = CONFIG_PERMISSION_COLUMN_LABEL;
};

auto permissions_check_actor_behaviour(
  const Pid& self,
  StatePtr& _state,
  const Message& message
) -> ResultUnion
{
  if (not _state)
  {
    _state = std::make_shared<PermissionsCheckActorState>();

    auto& state = *(std::static_pointer_cast<PermissionsCheckActorState>(_state));

    // Parse (& copy) the permission check request intent flatbuffer
    state.permissions_check_request_intent_mutable_buf = parse_request_intent(
      embedded_files::permissions_check_request_intent_req_fb
    );
    // Extract the RequestIntent root
    state.permissions_check_request_intent = GetMutableRequestIntent(
      state.permissions_check_request_intent_mutable_buf.data()
    );
    // Set the destination Pid for response messages
    update_uuid(state.permissions_check_request_intent->mutable_to_pid(), self);

    // Parse (& copy) the permission check request intent flatbuffer
    state.activity_request_intent_mutable_buf = parse_request_intent(
      embedded_files::activity_request_intent_req_fb
    );
    // Extract the RequestIntent root
    state.activity_request_intent = GetMutableRequestIntent(
      state.activity_request_intent_mutable_buf.data()
    );
    // Set the destination Pid for response messages
    auto google_sheets_actor_pid = *(whereis("google_sheets"));
    update_uuid(state.activity_request_intent->mutable_to_pid(), google_sheets_actor_pid);

    state.permissions_check_query_mutable_buf.assign(
      embedded_files::permissions_check_query_gviz_fb.begin(),
      embedded_files::permissions_check_query_gviz_fb.end()
    );
    state.permissions_check_query = GetMutableQuery(
      state.permissions_check_query_mutable_buf.data()
    );
  }
  auto& state = *(std::static_pointer_cast<PermissionsCheckActorState>(_state));

  auto request_manager_actor_pid = *(whereis("request_manager"));
  auto display_actor_pid = *(whereis("display"));

  const Response* response;
  if (matches(message, "chunk", response))
  {
    printf("received chunk\n");

    printf("received chunk from gviz\n");
    const Datatable* datatable = flatbuffers::GetRoot<Datatable>(
      response->body()->data()
    );

    if (not state.ready_to_run)
    {
      auto did_update_all_ids = update_query_columns(
        datatable->cols(),
        state.permissions_check_query
      );
      if (did_update_all_ids)
      {
        state.ready_to_run = true;
      }
      else {
        ESP_LOGE(TAG, "Could not update all column IDs for query");
      }
    }

    if (
      state.ready_to_run
      and datatable
      and datatable->rows()
      and (datatable->rows()->Length() > 0)
    )
    {
      // Generate a User from the query results
      state.current_user_flatbuf = generate_user_from_query_results(
        state.permissions_check_query,
        datatable
      );
      printf("Updated logged in user\n");
    }

    if (response->code() < 400)
    {
      ESP_LOGI(TAG, "got chunk (%d): '%.*s'\n", response->code(), response->body()->size(), response->body()->data());
    }
    else {
      ESP_LOGE(TAG, "got chunk (%d): '%.*s'\n", response->code(), response->body()->size(), response->body()->data());
    }
  }

  else if (matches(message, "complete", response))
  {
    printf("permissions check complete\n");
    if (state.ready_to_run)
    {
      if (state.current_user_flatbuf.size() > 0)
      {
        const User* current_user = flatbuffers::GetRoot<User>(
          state.current_user_flatbuf.data()
        );

        // Show user details on OLED display
        auto show_user_details_display_intent_flatbuf = (
          generate_show_user_details_from_user(current_user)
        );

        send(
          display_actor_pid,
          "ShowUserDetails",
          show_user_details_display_intent_flatbuf
        );

        // Send Signed_In activity
        set_request_body(
          state.activity_request_intent_mutable_buf,
          generate_activity_json(
            state.machine_id_str,
            ActivityType::Signed_In,
            current_user
          )
        );

        send(
          request_manager_actor_pid,
          "request",
          state.activity_request_intent_mutable_buf
        );
      }
      else {
        ESP_LOGW(TAG, "Missing user before generating Activity request");
      }
    }
    else {
      ESP_LOGE(TAG, "Not all columns updated before generating Activity request");
      //throw std::runtime_error("Not all columns updated before generating Activity request");
    }

    ESP_LOGI(TAG, "got body (%d): '%.*s'\n", response->code(), response->body()->size(), response->body()->data());
  }

  else if (matches(message, "error", response))
  {
    if (response->code() == 401)
    {
      auto reauth_actor_pid = *(whereis("reauth"));
      send(reauth_actor_pid, "reauth");
    }

    if (response->code() < 0)
    {
      ESP_LOGE(TAG, "Fatal error (%d): '%.*s'\n", response->code(), response->body()->size(), response->body()->data());
      throw std::runtime_error("Fatal error");
    }
    ESP_LOGE(TAG, "got error (%d): '%.*s'\n", response->code(), response->body()->size(), response->body()->data());
  }

  else if (message.type()->string_view() == "update_columns")
  {
    // Send the request intent message to the request manager actor
    send(
      request_manager_actor_pid,
      "request",
      state.permissions_check_request_intent_mutable_buf
    );
  }

  else if (message.type()->string_view() == "tag_seen")
  {
    const auto tag_id_str = string_view{
      reinterpret_cast<const char*>(message.payload()->data()),
      message.payload()->size()
    };
    auto query_str = generate_permissions_check_query_string(
      state.permissions_check_query,
      tag_id_str
    );

    // Set the 'tq' query arg
    set_query_arg(
      state.permissions_check_request_intent_mutable_buf,
      "tq",
      query_str
    );

    // Send the request intent message to the request manager actor
    send(
      request_manager_actor_pid,
      "request",
      state.permissions_check_request_intent_mutable_buf
    );
  }

  else if (message.type()->string_view() == "tag_lost")
  {
    // Clear the 'tq' query arg
    // Do not allow (possible, future) bugs to affect this user
    set_query_arg(
      state.permissions_check_request_intent_mutable_buf,
      "tq",
      ""
    );

    // Send a Signed_Out activity message
    if (state.current_user_flatbuf.size() > 0)
    {
      const User* current_user = flatbuffers::GetRoot<User>(
        state.current_user_flatbuf.data()
      );

      set_request_body(
        state.activity_request_intent_mutable_buf,
        generate_activity_json(
          state.machine_id_str,
          ActivityType::Signed_Out,
          current_user
        )
      );

      // Reset stored current_user
      //state.current_user_flatbuf.reset();
      state.current_user_flatbuf = UserFlatbuffer{};

      send(
        request_manager_actor_pid,
        "request",
        state.activity_request_intent_mutable_buf
      );
    }
  }

  else if (message.type()->string_view() == "reauth")
  {
    const auto access_token_str = string_view{
      reinterpret_cast<const char*>(message.payload()->data()),
      message.payload()->size()
    };

    // Use access_token to auth spreadsheet Users query request
    set_query_arg(
      state.permissions_check_request_intent_mutable_buf,
      "access_token",
      access_token_str
    );
  }

  return Ok;
}
