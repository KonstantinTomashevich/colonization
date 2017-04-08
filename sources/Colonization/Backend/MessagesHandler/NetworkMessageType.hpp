#pragma once
namespace Colonization
{
enum NetworkMessageType
{
    // CTS -- client to server message.
    CTS_NETWORK_MESSAGE_SEND_CHAT_MESSAGE = 100, // Data: message (String).
    CTS_NETWORK_MESSAGE_SEND_PRIVATE_MESSAGE = 101, // Data: message (String), ...recieviers... (String's).
    CTS_NETWORK_MESSAGE_SEND_PLAYER_ACTION = 102, // Data: actionType (int), actionDataBuffer (VectorBuffer ... [data]).
    CTS_NETWORK_MESSAGE_SEND_IS_PLAYER_READY_FOR_START = 103, // Data: isPlayerReadyForStart (bool).
    CTS_NETWORK_MESSAGE_RESELECT_PLAYER_COLOR = 104, // Data: playerColor (Color).

    // STC -- server to client message.
    STC_NETWORK_MESSAGE_SEND_PLAYER_STATS = 200, // Data: gold (float), points (float).
    STC_NETWORK_MESSAGE_CHAT_MESSAGE = 201, // Data: isPrivate (bool), senderName (String), message (String).
    STC_NETWORK_MESSAGE_TEXT_INFO_FROM_SERVER = 202, // Data: message (String).
    STC_NETWORK_MESSAGE_SEND_GAME_STATE = 203, // Data: int (GameState [enum]).
    STC_NETWORK_MESSAGE_GAME_ENDED = 204 // Data: winnerName (String), victoryType (String), victoryInfo (String).
};
}
