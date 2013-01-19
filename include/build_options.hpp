#ifndef _BUILD_OPTIONS_HPP_
#define _BUILD_OPTIONS_HPP_

#define BUILD_AUDIO_TUTORIAL
#define BUILD_VIDEO_PLAYER_TAB
#define BUILD_REPOSITORY_TAB
#define BUILD_DOCUMENTATION_TAB
#define BUILD_DECLARATIVE_TAB
// #define BUILD_DEVELOPER_TOOLS

// This option will enable GUI logging. Disables console output as a side-effect
// #define ENABLE_LOG_WINDOW

#ifndef QT_PHONON_LIB
#undef BUILD_AUDIO_TUTORIAL
#undef BUILD_VIDEO_PLAYER_TAB
#endif

#ifndef QT_DECLARATIVE_LIB
#undef BUILD_DECLARATIVE_TAB
#endif

#endif
