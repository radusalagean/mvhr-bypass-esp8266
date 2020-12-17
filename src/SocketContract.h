#ifndef SOCKET_CONTRACT_H
#define SOCKET_CONTRACT_H

#define SOCKET_KEY_EVENT "event"
#define SOCKET_KEY_DATA "data"
#define SOCKET_KEY_STATE "state"
#define SOCKET_KEY_TEMPERATURES "temperatures"

// ESP -> Socket client
#define SOCKET_EVENT_RESPONSE_INIT_DATA "response_init_data"
#define SOCKET_EVENT_RESPONSE_STATE "response_state"
#define SOCKET_EVENT_RESPONSE_TEMPERATURES "response_temperatures"
#define SOCKET_EVENT_RESPONSE_CONNECTION_BUSY "response_connection_busy"

// Socket client -> ESP
#define SOCKET_EVENT_REQUEST_HR_MODE_AUTO "request_hr_mode_auto"
#define SOCKET_EVENT_REQUEST_HR_MODE_MANUAL "request_hr_mode_manual"
#define SOCKET_EVENT_REQUEST_ENABLE_HR "request_enable_hr"
#define SOCKET_EVENT_REQUEST_DISABLE_HR "request_disable_hr"
#define SOCKET_EVENT_REQUEST_APPLY_STATE_TEMPERATURES "request_apply_state_temperatures"

#endif