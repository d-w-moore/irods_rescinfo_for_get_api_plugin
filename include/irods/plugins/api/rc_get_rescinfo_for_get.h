#ifndef IRODS_RC_PROJECT_TEMPLATE_H
#define IRODS_RC_PROJECT_TEMPLATE_H

struct RcComm;
struct DataObjInp;

/* This is defined and used statically in its own compilation unit, */
/* since currently there is a problem with linking in the rc_* call */

//extern "C" int rc_get_rescinfo_for_get(struct RcComm* _comm, const DataObjInp* _message, char** _response);

#endif // IRODS_RC_PROJECT_TEMPLATE_H
