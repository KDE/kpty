/*
    This file is part of the KDE libraries
    SPDX-FileCopyrightText: 2003, 2007 Oswald Buddenhagen <ossi@kde.org>
    SPDX-FileCopyrightText: 2022 Harald Sitter <sitter@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef kpty_h
#define kpty_h

#include "kpty_export.h"

#include <qglobal.h>

#include <memory>

class KPtyPrivate;
struct termios;

/*!
 * \class KPty
 * \inmodule KPty
 *
 * \brief Provides primitives for opening & closing a pseudo TTY pair, assigning the
 * controlling TTY, utmp registration and setting various terminal attributes.
 */
class KPTY_EXPORT KPty
{
    Q_DECLARE_PRIVATE(KPty)

public:
    /*!
     * Constructor
     */
    KPty();

    /*!
     * Destructor:
     *
     * If the pty is still open, it will be closed. Note, however, that
     * an utmp registration is not undone.
     */
    ~KPty();

    KPty(const KPty &) = delete;
    KPty &operator=(const KPty &) = delete;

    /*!
     * Create a pty master/slave pair.
     *
     * Returns true if a pty pair was successfully opened
     */
    bool open();

    /*!
     * Open using an existing pty master.
     *
     * \a fd an open pty master file descriptor.
     *   The ownership of the fd remains with the caller;
     *   it will not be automatically closed at any point.
     *
     * Returns true if a pty pair was successfully opened
     */
    bool open(int fd);

    /*!
     * Close the pty master/slave pair.
     */
    void close();

    /*!
     * Close the pty slave descriptor.
     *
     * When creating the pty, KPty also opens the slave and keeps it open.
     * Consequently the master will never receive an EOF notification.
     * Usually this is the desired behavior, as a closed pty slave can be
     * reopened any time - unlike a pipe or socket. However, in some cases
     * pipe-alike behavior might be desired.
     *
     * After this function was called, slaveFd() and setCTty() cannot be
     * used.
     */
    void closeSlave();

    /*!
     * Open the pty slave descriptor.
     *
     * This undoes the effect of closeSlave().
     *
     * Returns true if the pty slave was successfully opened
     */
    bool openSlave();

    /*!
     * Whether this will be a controlling terminal
     *
     * This is on by default.
     * Disabling the controllig aspect only makes sense if another process will
     * take over control or there is nothing to control or for technical reasons
     * control cannot be set (this notably is the case with flatpak-spawn when
     * used inside a sandbox).
     *
     * \a enable whether to enable ctty set up
     */
    void setCTtyEnabled(bool enable);

    /*!
     * Creates a new session and process group and makes this pty the
     * controlling tty.
     */
    void setCTty();

    /*!
     * Creates an utmp entry for the tty.
     * This function must be called after calling setCTty and
     * making this pty the stdin.
     *
     * \a user the user to be logged on
     *
     * \a remotehost the host from which the login is coming. This is
     *  not the local host. For remote logins it should be the hostname
     *  of the client. For local logins from inside an X session it should
     *  be the name of the X display. Otherwise it should be empty.
     */
    void login(const char *user = nullptr, const char *remotehost = nullptr);

    /*!
     * Removes the utmp entry for this tty.
     */
    void logout();

    /*!
     * Wrapper around tcgetattr(3).
     *
     * This function can be used only while the PTY is open.
     * You will need an #include <termios.h> to do anything useful
     * with it.
     *
     * \a ttmode a pointer to a termios structure.
     *  Note: when declaring ttmode, struct ::termios must be used -
     *  without the '::' some version of HP-UX thinks, this declares
     *  the struct in your class, in your method.
     *
     * Returns \c true on success, false otherwise
     */
    bool tcGetAttr(struct ::termios *ttmode) const;

    /*!
     * Wrapper around tcsetattr(3) with mode TCSANOW.
     *
     * This function can be used only while the PTY is open.
     *
     * \a ttmode a pointer to a termios structure.
     *
     * Returns true on success, false otherwise. Note that success means
     *  that at least one attribute could be set.
     */
    bool tcSetAttr(struct ::termios *ttmode);

    /*!
     * Change the logical (screen) size of the pty.
     * The default is 24 lines by 80 columns in characters, and zero pixels.
     *
     * This function can be used only while the PTY is open.
     *
     * \a lines the number of character rows
     *
     * \a columns the number of character columns
     *
     * \a height the view height in pixels
     *
     * \a width the view width in pixels
     *
     * Returns true on success, false otherwise
     *
     * \since 5.93
     */
    bool setWinSize(int lines, int columns, int height, int width);

    /*!
     * \overload
     * Change the logical (screen) size of the pty.
     * The pixel size is set to zero.
     */
    bool setWinSize(int lines, int columns);

    /*!
     * Set whether the pty should echo input.
     *
     * Echo is on by default.
     * If the output of automatically fed (non-interactive) PTY clients
     * needs to be parsed, disabling echo often makes it much simpler.
     *
     * This function can be used only while the PTY is open.
     *
     * \a echo true if input should be echoed.
     *
     * Returns true on success, false otherwise
     */
    bool setEcho(bool echo);

    /*!
     * Returns the name of the slave pty device.
     *
     * This function should be called only while the pty is open.
     */
    const char *ttyName() const;

    /*!
     * Returns the file descriptor of the master pty
     *
     * This function should be called only while the pty is open.
     */
    int masterFd() const;

    /*!
     * Returns the file descriptor of the slave pty
     *
     * This function should be called only while the pty slave is open.
     */
    int slaveFd() const;

protected:
    KPTY_NO_EXPORT explicit KPty(KPtyPrivate *d);

    std::unique_ptr<KPtyPrivate> const d_ptr;
};

#endif
