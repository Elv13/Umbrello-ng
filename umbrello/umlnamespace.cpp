/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2007                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#include "umlnamespace.h"
#include <qregexp.h>
#include <kdebug.h>

namespace Uml {

/**
 * Function for comparing tags in XMI files.
 */
bool tagEq (const QString& inTag, const QString& inPattern) {
    QString tag = inTag;
    QString pattern = inPattern;
    tag.remove( QRegExp("^\\w+:") );  // remove leading "UML:" or other
    int patSections = pattern.count('.') + 1;
    QString tagEnd = tag.section('.', -patSections);
    return (tagEnd.toLower() == pattern.toLower());
}

/**
 * Convert Visibility value into QString representation.
 *
 * @param value       Value to convert
 * @param mnemonic    If true then return a single character:
 *              "+" for public, "-" for private,
 *              "#" for protected or "~" for implementation
 */
QString Visibility::toString(Value value, bool mnemonic) {
    switch (value) {
    case Protected:
        return (mnemonic ? "#" : "protected");
        break;
    case Private:
        return (mnemonic ? "-" : "private");
        break;
    case Implementation:
        return (mnemonic ? "~" : "implementation");
        break;
    case Public:
    default:
        return (mnemonic ? "+" : "public");
        break;
    }
}

Visibility Visibility::fromString(const QString& vis) {
    if (vis == "public" || vis == "+")
        return Visibility(Public);
    else if (vis == "protected" || vis == "#")
        return Visibility(Protected);
    else if (vis == "private" || vis == "-")
        return Visibility(Private);
    else if (vis == "~")
        return Visibility(Implementation);
    else if (vis == "signals")
        return Visibility(Protected);
    else if (vis == "class")
        return Visibility(Private);
    else
        return Visibility(Public);
}

Visibility::Visibility(): _v(Public) {
}

Visibility::Visibility(Value v): _v(v) {
}

/**
 * Convert Visibility value into QString representation.
 *
 * @param mnemonic    If true then return a single character:
 *              "+" for public, "-" for private,
 *              "#" for protected or "~" for implementation
 */
QString Visibility::toString(bool mnemonic) const {
    return toString(_v, mnemonic);
}

Visibility::operator Visibility::Value() const {
    return _v;
}

}  // end namespace Uml

