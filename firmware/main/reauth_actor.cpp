#include "actors.h"

#include "requests.h"

#include "embedded_files.h"

#include "esp_log.h"

#include <string>
#include <experimental/string_view>

using namespace ActorModel;
using namespace Requests;

constexpr char TAG[] = "reauth_actor";

using string = std::string;
using string_view = std::experimental::string_view;

struct ReauthActorState
{
  MutableRequestIntentFlatbuffer reauth_request_intent_mutable_buf;
};

auto reauth_actor_behaviour(
  const Pid& self,
  StatePtr& _state,
  const Message& message
) -> ResultUnion
{
  if (not _state)
  {
    _state = std::make_shared<ReauthActorState>();

    auto& state = *(std::static_pointer_cast<ReauthActorState>(_state));

    state.reauth_request_intent_mutable_buf = parse_request_intent(
      embedded_files::reauth_request_intent_req_fb,
      self
    );
  }
  auto& state = *(std::static_pointer_cast<ReauthActorState>(_state));

  auto request_manager_actor_pid = *(whereis("request_manager"));

  const Response* response;
  auto reauth_request_intent_id = get_request_intent_id(
    state.reauth_request_intent_mutable_buf
  );

  if (matches(message, "chunk", response, reauth_request_intent_id))
  {
    if (response->code() < 400)
    {
      auto access_token_str = string_view{
        response->body()->data(),
        response->body()->size()
      };
      auto permissions_check_actor_pid = *(whereis("permissions_check"));
      send(permissions_check_actor_pid, "reauth", access_token_str);

      auto google_sheets_actor_pid = *(whereis("google_sheets"));
      send(google_sheets_actor_pid, "reauth", access_token_str);

      auto firmware_update_actor_pid = *(whereis("firmware_update"));
      send(firmware_update_actor_pid, "reauth", access_token_str);
    }
    else {
      ESP_LOGE(TAG, "got error chunk (%d): '%.*s'\n", response->code(), response->body()->size(), response->body()->data());
    }

    return Ok;
  }

  else if (matches(message, "complete", response, reauth_request_intent_id))
  {
    // Already processed access_token(s) via chunk messages
    return Ok;
  }

  else if (matches(message, "error", response, reauth_request_intent_id))
  {
    if (response->code() < 0)
    {
      ESP_LOGE(TAG, "Fatal error (%d): '%.*s'\n", response->code(), response->body()->size(), response->body()->data());
      throw std::runtime_error("Fatal error");
    }
    ESP_LOGE(TAG, "got error (%d): '%.*s'\n", response->code(), response->body()->size(), response->body()->data());

    return Ok;
  }

  else if (matches(message, "reauth"))
  {
    printf("send reauth request here\n");

    // Send the request intent message to the request manager actor
    send(
      request_manager_actor_pid,
      "request",
      state.reauth_request_intent_mutable_buf
    );

    return Ok;
  }

  return Unhandled;
}
