#include "irods/plugins/api/rc_get_rescinfo_for_get.h"

#include "irods/plugins/api/get_rescinfo_for_get_common.h" // For API plugin number.

#include <irods/procApiRequest.h>
#include <irods/rodsErrorTable.h>

auto rc_get_rescinfo_for_get(RcComm* _comm, const dataObjInp* _message, char** _response) -> int
{
	if (!_message || !_response) {
		return SYS_INVALID_INPUT_PARAM;
	}

	return procApiRequest(_comm,
	                      IRODS_APN_GET_RESCINFO_FOR_GET,
	                      _message, // NOLINT(cppcoreguidelines-pro-type-const-cast)
	                      nullptr,
	                      reinterpret_cast<void**>(_response), // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
	                      nullptr);
} // rc_get_rescinfo_for_get
