#pragma once

#include <QDataStream>
#include <QString>
#include <QVector>
#include <QMap>
#include "datamanager.h"

// Stream operators for User
inline QDataStream& operator<<(QDataStream& out, const User& user) {
    out << user.ID << user.Name << user.Password << user.IsAdmin;
    return out;
}

inline QDataStream& operator>>(QDataStream& in, User& user) {
    in >> user.ID >> user.Name >> user.Password >> user.IsAdmin;
    return in;
}

// Stream operators for Date
inline QDataStream& operator<<(QDataStream& out, const Date& date) {
    out << date.day << date.month << date.year << date.hour << date.minute;
    return out;
}

inline QDataStream& operator>>(QDataStream& in, Date& date) {
    in >> date.day >> date.month >> date.year >> date.hour >> date.minute;
    return in;
}

// Stream operators for Result
inline QDataStream& operator<<(QDataStream& out, const Result& result) {
    out << result.ID << result.TeamHomeGoals << result.TeamAwayGoals;
    return out;
}

inline QDataStream& operator>>(QDataStream& in, Result& result) {
    in >> result.ID >> result.TeamHomeGoals >> result.TeamAwayGoals;
    return in;
}

// Stream operators for Match
inline QDataStream& operator<<(QDataStream& out, const Match& match) {
    out << match.ID << match.TeamHomeID << match.TeamAwayID 
        << match.StartDate << match.result << match.Stadium << match.played;
    return out;
}

inline QDataStream& operator>>(QDataStream& in, Match& match) {
    in >> match.ID >> match.TeamHomeID >> match.TeamAwayID 
       >> match.StartDate >> match.result >> match.Stadium >> match.played;
    return in;
}

// Stream operators for Player
inline QDataStream& operator<<(QDataStream& out, const Player& player) {
    out << player.ID << player.Name << player.BirthDate << player.Nationality;
    return out;
}

inline QDataStream& operator>>(QDataStream& in, Player& player) {
    in >> player.ID >> player.Name >> player.BirthDate >> player.Nationality;
    return in;
}

// Stream operators for Team
inline QDataStream& operator<<(QDataStream& out, const Team& team) {
    out << team.ID << team.Name << team.Players << team.Stadium;
    return out;
}

inline QDataStream& operator>>(QDataStream& in, Team& team) {
    in >> team.ID >> team.Name >> team.Players >> team.Stadium;
    return in;
}

// Stream operators for TableRecord
inline QDataStream& operator<<(QDataStream& out, const TableRecord& record) {
    out << record.ID << record.No << record.TeamName << record.Points 
        << record.GoalsFor << record.GoalsAgainst << record.GoalDifference
        << record.Wins << record.Draws << record.Losses << record.Played;
    return out;
}

inline QDataStream& operator>>(QDataStream& in, TableRecord& record) {
    in >> record.ID >> record.No >> record.TeamName >> record.Points 
       >> record.GoalsFor >> record.GoalsAgainst >> record.GoalDifference
       >> record.Wins >> record.Draws >> record.Losses >> record.Played;
    return in;
}

// Stream operators for League
inline QDataStream& operator<<(QDataStream& out, const League& league) {
    out << league.ID << league.Name << league.Teams << league.Matches 
        << league.Table << league.SeasonStarted << league.SeasonStartDate;
    return out;
}

inline QDataStream& operator>>(QDataStream& in, League& league) {
    in >> league.ID >> league.Name >> league.Teams >> league.Matches 
       >> league.Table >> league.SeasonStarted >> league.SeasonStartDate;
    return in;
} 