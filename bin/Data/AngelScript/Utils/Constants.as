const String EVENT_GAME_STATE_CHANGED ("GameStateChanged");
namespace GameStateChanged
{
    const StringHash OLD_GAME_STATE ("OldGameState");
    const StringHash NEW_GAME_STATE ("NewGameState");
}

const String EVENT_NEW_CHAT_MESSAGE ("NewChatMessage");
namespace NewChatMessage
{
    const StringHash IS_PRIVATE ("IsPrivate");
    const StringHash SENDER ("Sender");
    const StringHash MESSAGE ("Message");
    const StringHash TIME_STAMP ("TimeStamp");
}

const String EVENT_NEW_NETWORK_TASK ("NewNetworkTask");
namespace NewNetworkTask
{
    const StringHash TASK_TYPE ("TaskType");
    const StringHash MESSAGE_BUFFER ("MessageBuffer");
}

const String EVENT_GO_TO_MAIN_MENU_REQUEST ("GoToMainMenuRequest");
namespace GoToMainMenuRequest
{
}

const String EVENT_DIPLOMACY_INFO ("DiplomacyInfo");
namespace DiplomacyInfo
{
    const StringHash TYPE ("Type");
    const StringHash DATA ("Data");
}

const String EVENT_DIPLOMACY_OFFER ("DiplomacyOffer");
namespace DiplomacyOffer
{
    const StringHash TYPE ("Type");
    const StringHash ID ("Id");
    const StringHash UNTIL_AUTODECLINE ("UntilAutodecline");
    const StringHash DATA ("Data");
}

const String EVENT_GAME_ENDED ("GameEnded");
namespace GameEnded
{
    const StringHash WINNER_NAME ("WinnerName");
    const StringHash VICTORY_TYPE ("VictoryType");
    const StringHash VICTORY_INFO ("VictoryInfo");
}

const String EVENT_SHOW_FUNCTIONAL_WINDOW_REQUEST ("ShowFunctionalWindowRequest");
namespace ShowFunctionalWindowRequest
{
    const StringHash WINDOW_NAME ("WindowName");
}

const String EVENT_SHOW_WINDOW_REQUEST ("ShowWindowRequest");
namespace ShowWindowRequest
{
    const StringHash WINDOW_NAME ("WindowName");
}

const String EVENT_START_GAME_REQUEST ("StartGameRequest");
namespace StartGameRequest
{
    const StringHash NICKNAME ("Nickname");
    const StringHash COLOR ("Color");
    const StringHash SELECTED_MAP_FOLDER ("SelectedMapFolder");
    const StringHash SELECTED_MAP_INFO ("SelectedMapInfo");
}

const String EVENT_JOIN_GAME_REQUEST ("JoinGameRequest");
namespace JoinGameRequest
{
    const StringHash NICKNAME ("Nickname");
    const StringHash COLOR ("Color");
    const StringHash ADRESS ("Adress");
    const StringHash PORT ("Port");
}

const String EVENT_EXIT_REQUEST ("ExitRequest");
namespace ExitRequest
{
}

namespace ScriptMainVars
{
    const StringHash PLAYER_NAME ("PlayerName");
    const StringHash GOLD ("Gold");
    const StringHash POINTS ("Points");
    const StringHash IS_ADMIN ("IsAdmin");
}

const int COLONIZATORS_EXPEDITION_SIZE = 100;
const uint NEW_GAME_DEFAULT_SERVER_PORT = 13768;
const float DEFAULT_RAYCAST_RAY_LENGTH = 300.0f;
const float DEFAULT_INVESTITION_SIZE = 100.0f;
const int DEFAULT_ARMY_SOLDIERS_COUNT = 50;

const Color NEUTRAL_COLOR = Color (0.5f, 0.5f, 0.5f, 1.0f);
const String MAPS_FOLDER ("Maps/");
const String FILESYSTEM_PREFIX ("../Data/");
const String EMPTY_FILTER ("");
const String MAP_INFO_FILE ("/info.xml");
const String DEFAULT_SCREEN_SHOTS_FOLDER ("../ScreenShots/");

const int DIPLOMACY_MESSAGE_INFO = 1;
const int DIPLOMACY_MESSAGE_OFFER = 2;
