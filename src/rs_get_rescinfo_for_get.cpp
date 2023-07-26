#include "irods/plugins/api/rs_get_rescinfo_for_get.hpp"

#include "irods/plugins/api/get_rescinfo_for_get_common.h" // For API plugin number.

#include <irods/rodsErrorTable.h>
#include <irods/irods_server_api_call.hpp>

auto rs_get_rescinfo_for_get(RsComm* _comm, const char* _message, char** _response) -> int
{
	if (!_message || !_response) {
		return SYS_INVALID_INPUT_PARAM;
	}

	return irods::server_api_call_without_policy(APN_PROJECT_TEMPLATE, _comm, _message, _response);
} // rs_get_rescinfo_for_get
