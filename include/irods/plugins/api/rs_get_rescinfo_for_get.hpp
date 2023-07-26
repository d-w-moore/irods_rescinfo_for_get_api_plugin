#ifndef IRODS_RS_PROJECT_TEMPLATE_HPP
#define IRODS_RS_PROJECT_TEMPLATE_HPP

struct RsComm;

int rs_get_rescinfo_for_get(RsComm* _comm, dataObjInp_t* dataObjInp, char** _response);

#endif // IRODS_RS_PROJECT_TEMPLATE_HPP
