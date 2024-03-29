#include "DebugEngineGdb.hpp"

#include "AppState.hpp"

DebugEngineGdb::DebugEngineGdb() {
}

DebugEngineGdb::~DebugEngineGdb() {
    Shutdown();
}

void DebugEngineGdb::Start() {
    const char* gdbExePath = gAppConf->gdbExePath;
    this->conn = Iu_GdbInit(gdbExePath);
}

void DebugEngineGdb::Shutdown() {
    if (conn) {
        Iu_GdbShutdown(conn);
        conn = nullptr;
    }
}

Callstack& DebugEngineGdb::GetCallstack() {
    return callstack;
}
