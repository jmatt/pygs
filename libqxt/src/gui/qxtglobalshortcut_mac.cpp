/****************************************************************************
 **
 ** Copyright (C) Qxt Foundation. Some rights reserved.
 **
 ** This file is part of the QxtGui module of the Qxt library.
 **
 ** This library is free software; you can redistribute it and/or modify it
 ** under the terms of the Common Public License, version 1.0, as published
 ** by IBM, and/or under the terms of the GNU Lesser General Public License,
 ** version 2.1, as published by the Free Software Foundation.
 **
 ** This file is provided "AS IS", without WARRANTIES OR CONDITIONS OF ANY
 ** KIND, EITHER EXPRESS OR IMPLIED INCLUDING, WITHOUT LIMITATION, ANY
 ** WARRANTIES OR CONDITIONS OF TITLE, NON-INFRINGEMENT, MERCHANTABILITY OR
 ** FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** You should have received a copy of the CPL and the LGPL along with this
 ** file. See the LICENSE file and the cpl1.0.txt/lgpl-2.1.txt files
 ** included with the source distribution for more information.
 ** If you did not receive a copy of the licenses, contact the Qxt Foundation.
 **
 ** <http://libqxt.org>  <foundation@libqxt.org>
 **
 ****************************************************************************/
#include <Carbon/Carbon.h>
#include "qxtglobalshortcut_p.h"
#include <QMap>
#include <QHash>
#include <QtDebug>
#include <QApplication>

typedef QPair<uint, uint> Identifier;
static QMap<quint32, EventHotKeyRef> keyRefs;
static QHash<Identifier, quint32> keyIDs;
static quint32 hotKeySerial = 0;
static bool qxt_mac_handler_installed = false;

OSStatus qxt_mac_handle_hot_key(EventHandlerCallRef nextHandler, EventRef event, void* data)
{
    Q_UNUSED(nextHandler);
    Q_UNUSED(data);
    if (GetEventClass(event) == kEventClassKeyboard && GetEventKind(event) == kEventHotKeyPressed)
    {
        EventHotKeyID keyID;
        GetEventParameter(event, kEventParamDirectObject, typeEventHotKeyID, NULL, sizeof(keyID), NULL, &keyID);
        Identifier id = keyIDs.key(keyID.id);
        QxtGlobalShortcutPrivate::activateShortcut(id.second, id.first);
    }
    return noErr;
}

quint32 QxtGlobalShortcutPrivate::nativeModifiers(Qt::KeyboardModifiers modifiers)
{
    quint32 native = 0;
    if (modifiers & Qt::ShiftModifier)
        native |= shiftKeyBit;
    if (modifiers & Qt::ControlModifier)
        native |= cmdKey;
    if (modifiers & Qt::AltModifier)
        native |= optionKey;
    if (modifiers & Qt::MetaModifier)
        native |= controlKey;
    if (modifiers & Qt::KeypadModifier)
        native |= kEventKeyModifierNumLockMask;
    return native;
}

/* Returns string representation of key, if it is printable.
 * Ownership follows the Create Rule; that is, it is the caller's
 * responsibility to release the returned object. */
CFStringRef createStringForKey(CGKeyCode keyCode)
{
    TISInputSourceRef currentKeyboard = TISCopyCurrentKeyboardInputSource();
    CFDataRef layoutData =
      (CFDataRef)TISGetInputSourceProperty(currentKeyboard,
                                  kTISPropertyUnicodeKeyLayoutData);
    const UCKeyboardLayout *keyboardLayout =
        (const UCKeyboardLayout *)CFDataGetBytePtr(layoutData);

    UInt32 keysDown = 0;
    UniChar chars[4];
    UniCharCount realLength;

    UCKeyTranslate(keyboardLayout,
                   keyCode,
                   kUCKeyActionDisplay,
                   0,
                   LMGetKbdType(),
                   kUCKeyTranslateNoDeadKeysBit,
                   &keysDown,
                   sizeof(chars) / sizeof(chars[0]),
                   &realLength,
                   chars);
    CFRelease(currentKeyboard);    

    return CFStringCreateWithCharacters(kCFAllocatorDefault, chars, 1);
}

/* Returns key code for given character via the above function, or UINT16_MAX
 * on error. 
 *
 * Created by meeselet (http://stackoverflow.com/users/217476/meeselet)
 * From: http://stackoverflow.com/questions/1918841/how-to-convert-ascii-character-to-cgkeycode
 */
CGKeyCode keyCodeForChar(const char c)
{
    static CFMutableDictionaryRef charToCodeDict = NULL;
    CGKeyCode code;
    UniChar character = c;
    CFStringRef charStr = NULL;

    /* Generate table of keycodes and characters. */
    if (charToCodeDict == NULL) {
        size_t i;
        charToCodeDict = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                   128,
                                                   &kCFCopyStringDictionaryKeyCallBacks,
                                                   NULL);
	if (charToCodeDict == NULL) return UINT16_MAX;

        /* Loop through every keycode (0 - 127) to find its current mapping. */
        for (i = 0; i < 128; ++i) {
            CFStringRef string = createStringForKey((CGKeyCode)i);
            if (string != NULL) {
                CFDictionaryAddValue(charToCodeDict, string, (const void *)i);
                CFRelease(string);
            }
        }
    }

    charStr = CFStringCreateWithCharacters(kCFAllocatorDefault, &character, 1);

    /* Our values may be NULL (0), so we need to use this function. */
    if (!CFDictionaryGetValueIfPresent(charToCodeDict, charStr,
                                       (const void **)&code)) {
      code = UINT16_MAX;
    }

    CFRelease(charStr);
    return code;
}  

quint32 QxtGlobalShortcutPrivate::nativeKeycode(Qt::Key key)
{
    UTF16Char ch;
    // Constants found in NSEvent.h from AppKit.framework
    if (key == Qt::Key_Up)			ch = 0xF700;
    else if (key == Qt::Key_Down)		ch = 0xF701;
    else if (key == Qt::Key_Left)		ch = 0xF702;
    else if (key == Qt::Key_Right)		ch = 0xF703;
    else if (key >= Qt::Key_F1 && key <= Qt::Key_F35)
        ch = key - Qt::Key_F1 + 0xF704;
    else if (key == Qt::Key_Insert)		ch = 0xF727;
    else if (key == Qt::Key_Delete)		ch = 0xF728;
    else if (key == Qt::Key_Home)		ch = 0xF729;
    else if (key == Qt::Key_End)		ch = 0xF72B;
    else if (key == Qt::Key_PageUp)		ch = 0xF72C;
    else if (key == Qt::Key_PageDown)	        ch = 0xF72D;
    else if (key == Qt::Key_Print)		ch = 0xF72E;
    else if (key == Qt::Key_ScrollLock)	ch = 0xF72F;
    else if (key == Qt::Key_Pause)		ch = 0xF730;
    else if (key == Qt::Key_SysReq)		ch = 0xF731;
    else if (key == Qt::Key_Stop)		ch = 0xF734;
    else if (key == Qt::Key_Menu)		ch = 0xF735;
    else if (key == Qt::Key_Select)		ch = 0xF741;
    else if (key == Qt::Key_Execute)		ch = 0xF742;
    else if (key == Qt::Key_Help)		ch = 0xF746;
    else if (key == Qt::Key_Mode_switch)	ch = 0xF747;
    else if (key == Qt::Key_Escape)		ch = 27;
    else if (key == Qt::Key_Return)		ch = 13;
    else if (key == Qt::Key_Enter)		ch = 3;
    else if (key == Qt::Key_Tab)		ch = 9;
    else					ch = key;
#if defined(QT_MAC_USE_COCOA)
    CGKeyCode macKeyCode = keyCodeForChar(QChar::toLower(ch));
    return macKeyCode;
#else
    KeyboardLayoutRef layout;
    KeyboardLayoutKind layoutKind;
    KLGetCurrentKeyboardLayout(&layout);
    KLGetKeyboardLayoutProperty(layout, kKLKind, const_cast<const void**>(reinterpret_cast<void**>(&layoutKind)));

    if (layoutKind == kKLKCHRKind)
    { // no Unicode available
        if (ch > 255) return 0;

        char* data;
        KLGetKeyboardLayoutProperty(layout, kKLKCHRData, const_cast<const void**>(reinterpret_cast<void**>(&data)));
        int ct = *reinterpret_cast<short*>(data + 258);
        for (int i = 0; i < ct; i++)
        {
            char* keyTable = data + 260 + 128 * i;
            for (int j = 0; j < 128; j++)
            {
                if (keyTable[j] == ch) return j;
            }
        }

        return 0;
    }

    char* data;
    KLGetKeyboardLayoutProperty(layout, kKLuchrData, const_cast<const void**>(reinterpret_cast<void**>(&data)));
    UCKeyboardLayout* header = reinterpret_cast<UCKeyboardLayout*>(data);
    UCKeyboardTypeHeader* table = header->keyboardTypeList;

    for (quint32 i=0; i < header->keyboardTypeCount; i++)
    {
        UCKeyStateRecordsIndex* stateRec = 0;
        if (table[i].keyStateRecordsIndexOffset != 0)
        {
            stateRec = reinterpret_cast<UCKeyStateRecordsIndex*>(data + table[i].keyStateRecordsIndexOffset);
            if (stateRec->keyStateRecordsIndexFormat != kUCKeyStateRecordsIndexFormat) stateRec = 0;
        }

        UCKeyToCharTableIndex* charTable = reinterpret_cast<UCKeyToCharTableIndex*>(data + table[i].keyToCharTableIndexOffset);
        if (charTable->keyToCharTableIndexFormat != kUCKeyToCharTableIndexFormat) continue;

        for (quint32 j=0; j < charTable->keyToCharTableCount; j++)
        {
            UCKeyOutput* keyToChar = reinterpret_cast<UCKeyOutput*>(data + charTable->keyToCharTableOffsets[j]);
            for (quint32 k=0; k < charTable->keyToCharTableSize; k++)
            {
                if (keyToChar[k] & kUCKeyOutputTestForIndexMask)
                {
                    long idx = keyToChar[k] & kUCKeyOutputGetIndexMask;
                    if (stateRec && idx < stateRec->keyStateRecordCount)
                    {
                        UCKeyStateRecord* rec = reinterpret_cast<UCKeyStateRecord*>(data + stateRec->keyStateRecordOffsets[idx]);
                        if (rec->stateZeroCharData == ch) return k;
                    }
                }
                else if (!(keyToChar[k] & kUCKeyOutputSequenceIndexMask) && keyToChar[k] < 0xFFFE)
                {
                    if (keyToChar[k] == ch) return k;
                }
            } // for k
        } // for j
    } // for i
#endif
    return 0;
}

bool QxtGlobalShortcutPrivate::registerShortcut(quint32 nativeKey, quint32 nativeMods)
{
    if (!qxt_mac_handler_installed)
    {
        EventTypeSpec t;
        t.eventClass = kEventClassKeyboard;
        t.eventKind = kEventHotKeyPressed;
        InstallApplicationEventHandler(&qxt_mac_handle_hot_key, 1, &t, NULL, NULL);
    }

    EventHotKeyID keyID;
    keyID.signature = 'cute';
    keyID.id = ++hotKeySerial;

    EventHotKeyRef ref = 0;
    bool rv = !RegisterEventHotKey(nativeKey, nativeMods, keyID, GetApplicationEventTarget(), 0, &ref);
    if (rv)
    {
        keyIDs.insert(Identifier(nativeMods, nativeKey), keyID.id);
        keyRefs.insert(keyID.id, ref);
    }
    return rv;
}

bool QxtGlobalShortcutPrivate::unregisterShortcut(quint32 nativeKey, quint32 nativeMods)
{
    Identifier id(nativeMods, nativeKey);
    if (!keyIDs.contains(id)) return false;

    EventHotKeyRef ref = keyRefs.take(keyIDs[id]);
    keyIDs.remove(id);
    return !UnregisterEventHotKey(ref);
}
