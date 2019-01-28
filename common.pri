CXX11COMPATIBILITY = $$(CXX11COMPATIBILITY)
isEmpty( CXX11COMPATIBILITY ) : {
    CONFIG += c++17
} else: {
    warning("Enabling C++11 compatibility mode")
    CONFIG += c++11
}

VERSION = 0.1

INCLUDEPATH += $$PWD
HEADERS += $$PWD/qtwitch_global.h

TWITCH_CLIENT_ID = $$(TWITCH_CLIENT_ID)
isEmpty( TWITCH_CLIENT_ID ): {
    error("Twitch API requests require a client id. Please get one via Twitch dashboard and specify it in TWITCH_CLIENT_ID environment variable.")
} else: {
    DEFINES += TWITCH_CLIENT_ID=\"\\\"$$TWITCH_CLIENT_ID\\\"\"
}

TWITCH_REDIRECT_URI = $$(TWITCH_REDIRECT_URI)
isEmpty( TWITCH_REDIRECT_URI ): {
    warning("Authorization requires a known redirect uri. If you want to perform user authorization, please set redirect uri on Twitch dashboard and specify it in TWITCH_REDIRECT_URI environment variable.")
    DEFINES += TWITCH_REDIRECT_URI=\"\\\"\\\"\"
} else: {
    DEFINES += TWITCH_REDIRECT_URI=\"\\\"$$TWITCH_REDIRECT_URI\\\"\"
}

debug {
    QMAKE_CXXFLAGS += -Wall -Werror
}

DEFINES += QTWITCH_VERSION=\"\\\"$$VERSION\\\"\"
