/*
 * Copyright 2012-2013 James Geboski <jgeboski@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _STEAM_API_H
#define _STEAM_API_H

#include "steam-auth.h"
#include "steam-friend.h"
#include "steam-http.h"

#define STEAM_API_HOST        "api.steampowered.com"
#define STEAM_COM_HOST        "steamcommunity.com"
#define STEAM_API_AGENT       "Steam App / " PACKAGE " / " PACKAGE_VERSION
#define STEAM_API_CLIENT_ID   "DE45CD61" /* Public mobile client id */
#define STEAM_API_KEEP_ALIVE  "15"       /* Max of 30 seconds */

#define STEAM_API_PATH_FRIEND_SEARCH "/ISteamUserOAuth/Search/v0001"
#define STEAM_API_PATH_FRIENDS       "/ISteamUserOAuth/GetFriendList/v0001"
#define STEAM_API_PATH_LOGON         "/ISteamWebUserPresenceOAuth/Logon/v0001"
#define STEAM_API_PATH_LOGOFF        "/ISteamWebUserPresenceOAuth/Logoff/v0001"
#define STEAM_API_PATH_MESSAGE       "/ISteamWebUserPresenceOAuth/Message/v0001"
#define STEAM_API_PATH_POLL          "/ISteamWebUserPresenceOAuth/Poll/v0001"
#define STEAM_API_PATH_SUMMARIES     "/ISteamUserOAuth/GetUserSummaries/v0001"

#define STEAM_COM_PATH_AUTH          "/mobilelogin/dologin/"
#define STEAM_COM_PATH_AUTH_RDIR     "/mobileloginsucceeded/"
#define STEAM_COM_PATH_CAPTCHA       "/public/captcha.php"
#define STEAM_COM_PATH_FRIEND_ADD    "/actions/AddFriendAjax/"
#define STEAM_COM_PATH_FRIEND_BLOCK  "/actions/BlockUserAjax/"
#define STEAM_COM_PATH_FRIEND_REMOVE "/actions/RemoveFriendAjax/"
#define STEAM_COM_PATH_KEY           "/mobilelogin/getrsakey/"
#define STEAM_COM_PATH_PROFILE       "/profiles/"

typedef enum   _SteamApiError     SteamApiError;
typedef enum   _SteamFriendState  SteamFriendState;
typedef enum   _SteamMessageType  SteamMessageType;
typedef enum   _SteamRelationship SteamRelationship;
typedef enum   _SteamState        SteamState;
typedef struct _SteamApi          SteamApi;
typedef struct _SteamMessage      SteamMessage;
typedef struct _SteamSummary      SteamSummary;

typedef void (*SteamApiFunc)     (SteamApi *api, GError *err,gpointer data);
typedef void (*SteamIDFunc)      (SteamApi *api, gchar *steamid, GError *err,
                                  gpointer data);
typedef void (*SteamListFunc)    (SteamApi *api, GSList *list, GError *err,
                                  gpointer data);
typedef void (*SteamSummaryFunc) (SteamApi *api, SteamSummary *ss, GError *err,
                                  gpointer data);

enum _SteamApiError
{
    STEAM_API_ERROR_AUTH = 0,
    STEAM_API_ERROR_FRIEND_ACCEPT,
    STEAM_API_ERROR_FRIEND_ADD,
    STEAM_API_ERROR_FRIEND_IGNORE,
    STEAM_API_ERROR_FRIEND_REMOVE,
    STEAM_API_ERROR_FRIEND_SEARCH,
    STEAM_API_ERROR_FRIENDS,
    STEAM_API_ERROR_KEY,
    STEAM_API_ERROR_LOGOFF,
    STEAM_API_ERROR_LOGON,
    STEAM_API_ERROR_RELOGON,
    STEAM_API_ERROR_MESSAGE,
    STEAM_API_ERROR_POLL,
    STEAM_API_ERROR_SUMMARIES,

    STEAM_API_ERROR_AUTH_CAPTCHA,
    STEAM_API_ERROR_AUTH_GUARD,
    STEAM_API_ERROR_EMPTY_REPLY,
    STEAM_API_ERROR_LOGON_EXPIRED,
    STEAM_API_ERROR_PARSER
};

enum _SteamFriendState
{
    STEAM_FRIEND_STATE_REMOVE    = 0,
    STEAM_FRIEND_STATE_IGNORE    = 1,
    STEAM_FRIEND_STATE_REQUEST   = 2,
    STEAM_FRIEND_STATE_ADD       = 3,
    STEAM_FRIEND_STATE_REQUESTED = 4,

    STEAM_FRIEND_STATE_NONE,
    STEAM_FRIEND_STATE_LAST
};

enum _SteamMessageType
{
    STEAM_MESSAGE_TYPE_SAYTEXT = 0,
    STEAM_MESSAGE_TYPE_EMOTE,
    STEAM_MESSAGE_TYPE_LEFT_CONV,
    STEAM_MESSAGE_TYPE_RELATIONSHIP,
    STEAM_MESSAGE_TYPE_STATE,
    STEAM_MESSAGE_TYPE_TYPING,

    STEAM_MESSAGE_TYPE_LAST
};

enum _SteamRelationship
{
    STEAM_RELATIONSHIP_FRIEND = 0,
    STEAM_RELATIONSHIP_IGNORE
};

enum _SteamState
{
    STEAM_STATE_OFFLINE = 0,
    STEAM_STATE_ONLINE  = 1,
    STEAM_STATE_BUSY    = 2,
    STEAM_STATE_AWAY    = 3,
    STEAM_STATE_SNOOZE  = 4,
    STEAM_STATE_TRADE   = 5,
    STEAM_STATE_PLAY    = 6,

    STEAM_STATE_LAST
};

struct _SteamApi
{
    gchar  *steamid;
    gchar  *umqid;
    gchar  *token;
    gchar  *sessid;
    gint64  lmid;

    SteamHttp *http;
    SteamAuth *auth;
};

struct _SteamMessage
{
    SteamMessageType  type;
    SteamSummary     *ss;

    gchar  *text;
    gint64  tstamp;
};

struct _SteamSummary
{
    SteamState        state;
    SteamFriendState  fstate;
    SteamRelationship relation;

    gchar *steamid;
    gchar *nick;
    gchar *fullname;
    gchar *game;
    gchar *server;
};

#define STEAM_API_ERROR steam_api_error_quark()

GQuark steam_api_error_quark(void);

SteamApi *steam_api_new(const gchar *umqid);

void steam_api_free(SteamApi *api);

void steam_api_refresh(SteamApi *api);

SteamMessage *steam_message_new(const gchar *steamid);

void steam_message_free(SteamMessage *sm);

SteamSummary *steam_summary_new(const gchar *steamid);

void steam_summary_free(SteamSummary *ss);

void steam_api_auth(SteamApi *api, const gchar *user, const gchar *pass,
                    const gchar *authcode, const gchar *captcha,
                    SteamApiFunc func, gpointer data);

void steam_api_friend_accept(SteamApi *api, const gchar *steamid,
                             const gchar *action, SteamIDFunc func,
                             gpointer data);

void steam_api_friend_add(SteamApi *api, const gchar *steamid,
                          SteamIDFunc func, gpointer data);

void steam_api_friend_ignore(SteamApi *api, const gchar *steamid,
                             gboolean ignore, SteamIDFunc func,
                             gpointer data);

void steam_api_friend_remove(SteamApi *api, const gchar *steamid,
                             SteamIDFunc func, gpointer data);

void steam_api_friend_search(SteamApi *api, const gchar *search, guint count,
                             SteamListFunc func, gpointer data);

void steam_api_friends(SteamApi *api, SteamListFunc func, gpointer data);

void steam_api_key(SteamApi *api, const gchar *user, SteamApiFunc func,
                   gpointer data);

void steam_api_logoff(SteamApi *api, SteamApiFunc func, gpointer data);

void steam_api_logon(SteamApi *api, SteamApiFunc func, gpointer data);

void steam_api_relogon(SteamApi *api, SteamApiFunc func, gpointer data);

void steam_api_message(SteamApi *api, const SteamMessage *sm,
                       SteamApiFunc func, gpointer data);

void steam_api_poll(SteamApi *api, SteamListFunc func, gpointer data);

void steam_api_summary(SteamApi *api, const gchar *steamid,
                       SteamSummaryFunc func, gpointer data);

gchar *steam_api_profile_url(const gchar *steamid);

const gchar *steam_message_type_str(SteamMessageType type);

const gchar *steam_state_str(SteamState state);

SteamState steam_state_from_str(const gchar *state);

#endif /* _STEAM_API_H */
