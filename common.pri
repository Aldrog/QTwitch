CONFIG += c++17
VERSION = 0.1

INCLUDEPATH += $$PWD
HEADERS += $$PWD/qtwitch_global.h

TWITCH_CLIENT_ID = $$(TWITCH_CLIENT_ID)
isEmpty( TWITCH_CLIENT_ID ): {
    error("Twitch API requests require a client id. Please get one via Twitch dashboard and specify it in TWITCH_CLIENT_ID environment variable.")
} else: {
    DEFINES += TWITCH_CLIENT_ID=\"\\\"$$TWITCH_CLIENT_ID\\\"\"
}

DEFINES += QTWITCH_VERSION=\"\\\"$$VERSION\\\"\"
