#include "server/common/net/connection.h"
#include "server/common/base/log.h"
#include "server/common/base/time_manager.h"

namespace pap_server_common_net {

#if defined(_PAP_BILLING) /* { */
const char* g_kModelName = "billing";    
const uint8_t g_kModelSaveLogId = kBillingLogFile;
#elif defined(_PAP_LOGIN) /* }{ */
const char* g_kModelName = "login";
const uint8_t g_kModelSaveLogId = kLoginLogFile;
#elif defined(_PAP_WORLD) /* }{ */
const char* g_kModelName = "world";
const uint8_t g_kModelSaveLogId = kWorldLogFile;
#elif defined(_PAP_SERVER) /* }{ */
const char* g_kModelName = "server";
const uint8_t g_kModelSaveLogId = kServerLogFile;
#endif /* } */

Connection::Connection(bool flag_isserver) {
  __ENTER_FUNCTION
    id_ = ID_INVALID;
    userid_ = ID_INVALID;
    managerid_ = ID_INVALID;
    socket_ = new pap_common_net::Socket();
    Assert(socket_);
    if (!flag_isserver) {
      socket_inputstream_ = new pap_common_net::SocketInputStream(socket_);
      Assert(socket_inputstream_);
      socket_outputstream_ = new pap_common_net::SocketOutputStream(socket_);
      Assert(socket_outputstream_);
    }
    else {
      socket_inputstream_ = new pap_common_net::SocketInputStream(
          socket_,
          SOCKETINPUT_BUFFERSIZE_DEFAULT,
          64 * 1024 * 1024
          );
      Assert(socket_inputstream_);
      socket_outputstream_ = new pap_common_net::SocketOutputStream(
          socket_,
          SOCKETOUTPUT_BUFFERSIZE_DEFAULT,
          64 * 1024 * 1024);
      Assert(socket_outputstream_);
    }
    isempty_ = true;
    isdisconnect_ = false;
    packetindex_ = 0;
  __LEAVE_FUNCTION
}

Connection::~Connection() {
  __ENTER_FUNCTION
    SAFE_DELETE(socket_outputstream_);
    SAFE_DELETE(socket_inputstream_);
    SAFE_DELETE(socket_);
  __LEAVE_FUNCTION
}

bool Connection::processinput() {
  __ENTER_FUNCTION
    using namespace pap_server_common_base;
    bool result = false;
    if (isdisconnect()) return true;
    try {
      uint32_t fillresult = socket_inputstream_->fill();
      if (static_cast<int32_t>(fillresult) <= SOCKET_ERROR) {
        char errormessage[FILENAME_MAX];
        memset(errormessage, '\0', sizeof(errormessage));
        socket_inputstream_->getlast_errormessage(
            errormessage, 
            static_cast<uint16_t>(sizeof(errormessage) - 1));
        Log::save_log(g_kModelName,
                      "[net](%d) Connection::processinput()"
                      " socket_inputstream_->fill() result: %d %s",
                      g_time_manager->tm_todword(), 
                      fillresult,
                      errormessage);
        result = false;
      }
      else {
        result = true;
      }
    }
    catch(...) {
      SaveCodeLog();
    }
    return result;
  __LEAVE_FUNCTION
    return false;
}

bool Connection::processoutput() {
  __ENTER_FUNCTION
    using namespace pap_server_common_base;
    bool result = false;
    if (isdisconnect()) return true;
    try {
      uint32_t size = socket_outputstream_->reallength();
      if (0 == size) return true;
      uint32_t flushresult = socket_outputstream_->flush();
      if (static_cast<int32_t>(flushresult) <= SOCKET_ERROR) {
        char errormessage[FILENAME_MAX];
        memset(errormessage, '\0', sizeof(errormessage));
        socket_inputstream_->getlast_errormessage(
            errormessage, 
            static_cast<uint16_t>(sizeof(errormessage) - 1));
        Log::save_log(g_kModelName,
                      "[net](%d) Connection::processoutput()"
                      " socket_outputstream_->flush() result: %d %s",
                      g_time_manager->tm_todword(), 
                      fillresult,
                      errormessage);
        result = false;
      }
      else {
        result = true;
      }
    }
    catch {
      SaveCodeLog();
    }
    return result;
  __LEAVE_FUNCTION
    return false;
}

bool Connection::processcommand(bool option) {
  __ENTER_FUNCTION
    bool result = false;
    char packetheader[PACKET_HEADERSIZE] = {'\0'};
    uint16_t packetid;
    uint32_t packetinfo, packetsize, packetindex;
    pap_common_net::Packet* packet = NULL;
    if (isdisconnect()) return true;
    try {
      if (option) { //执行选项操作
      }
      const uint8_t kExecuteCountPreTick = 12;
      uint32_t i;
      for (i = 0; i < kExecuteCountPreTick; ++i) {
        if (!socket_inputstream_->peek(&pakcetheader[0], PACKET_HEADERSIZE)) {
          //数据不能填充消息头
          break;
        }
        memcpy(&packetid, &packetheader[0], sizeof(uint16_t));
        memcpy(&packetinfo, &pakcetheader[sizeof(uint16_t)], sizeof(uint32_t));
        packetsize = GET_PACKETLENGTH(packetinfo);
        packetindex = GET_PACKETINDEX(packetinfo);
        if (packetid >= static_cast<uint16_t>(kPacketMax)) {
          return false;
        }
        try {
          if (socket_inputstream_->reallength() < 
              PACKET_HEADERSIZE + packetsize) {
            //message not receive full
            break;
          }

          //
        }
        catch(...) {
          SaveCodeLog();
        }
      }
    }
    catch(...) {
      SaveCodeLog();
    }
    return result;
  __LEAVE_FUNCTION
    return false;
}

bool Connection::sendpacket(pap_common_net::Packet* packet) {
  __ENTER_FUNCTION
    bool result = false;
    if (isdisconnect()) return true;
#if defined(_PAP_SERVER)
    uint32_t before_writesize = socket_outputstream_->reallength();
#endif
    result = socket_outputstream_->writepacket(packet);
    Assert(result);
#if defined(_PAP_SERVER)
    uint32_t after_writesize = socket_outputstream_->reallength();
    if (packet->getsize() != after_writesize - before_writesize - 6) {
      g_log->fast_save_log(g_kModelSaveLogId,
                           "[net] Connection::sendpacket() size error"
                           "id = %d(write: %d, should: %d)",
                           pakcet->getid(),
                           after_writesize - before_writesize - 6,
                           pakcet->getsize());
    }
    if (kPacketIdSCCharacterIdle == packet->getid()) {
      //save heartbeat
    }
#endif
    return result;
  __LEAVE_FUNCTION
    return false;
}

} //namespace pap_server_common_net