#include "logging_event_handler.h"
#include <ace/FILE_Connector.h>
#include <ace/Reactor.h>
#include <ace/os_include/os_netdb.h>
#include <cstring>
#include <iostream>

using namespace std;

ACE_HANDLE Logging_Event_Handler::get_handle() const
{
    Logging_Handler &h = const_cast<Logging_Handler&>(logging_handler_);
    return h.peer().get_handle();
}

int Logging_Event_Handler::open()
{
    static const char LOGFILE_SUFFIX[] = ".log";
    char filename[MAXHOSTNAMELEN + sizeof(LOGFILE_SUFFIX)];
    ACE_INET_Addr logging_peer_addr;

    logging_handler_.peer().get_remote_addr(logging_peer_addr);
    logging_peer_addr.get_host_name(filename, MAXHOSTNAMELEN);
    strcat(filename, LOGFILE_SUFFIX);

    ACE_FILE_Connector connector;
    connector.connect(log_file_,
                      ACE_FILE_Addr(filename),
                      0,
                      ACE_Addr::sap_any,
                      0,
                      O_RDWR | O_CREAT | O_APPEND,
                      ACE_DEFAULT_FILE_PERMS);
    return reactor()->register_handler(
        this, ACE_Event_Handler::READ_MASK);
}

int Logging_Event_Handler::handle_input(ACE_HANDLE)
{
    return logging_handler_.log_record();
}

int Logging_Event_Handler::handle_close(ACE_HANDLE, ACE_Reactor_Mask)
{
    logging_handler_.close();
    log_file_.close();
    delete this;
    return 0;
}
// vim: set et ts=4 sts=4 sw=4:
