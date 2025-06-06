/*
    This file is part of the KDE libraries
    SPDX-FileCopyrightText: 2007 Oswald Buddenhagen <ossi@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KPTYPROCESS_H
#define KPTYPROCESS_H

#include <KProcess>

#include "kpty_export.h"

#include <memory>

class KPtyDevice;

class KPtyProcessPrivate;

/*!
 * \class KPtyProcess
 * \inmodule KPty
 *
 * \brief This class extends KProcess by support for PTYs (pseudo TTYs).
 *
 * The PTY is opened as soon as the class is instantiated. Verify that
 * it was opened successfully by checking that pty()->masterFd() is not -1.
 *
 * The PTY is always made the process' controlling TTY.
 * Utmp registration and connecting the stdio handles to the PTY are optional.
 *
 * No attempt to integrate with QProcess' waitFor*() functions was made,
 * for it is impossible. Note that execute() does not work with the PTY, too.
 * Use the PTY device's waitFor*() functions or use it asynchronously.
 *
 * \note If you inherit from this class and use setChildProcessModifier() in
 * the derived class, you must call the childProcessModifier() of KPtyProcess
 * first (using setChildProcessModifier() in the derived class will "overwrite"
 * the childProcessModifier() std::function that was previously set by KPtyProcess).
 *
 * For example:
 * \code
 * class MyProcess : public KPtyProcess
 * {
 *     MyProcess()
 *     {
 *         auto parentChildProcModifier = KPtyProcess::childProcessModifier();
 *         setChildProcessModifier([parentChildProcModifier]() {
 *             // First call the parent class modifier function
 *             if (parentChildProcModifier) {
 *                 parentChildProcModifier();
 *             }
 *             // Then whatever extra code you need to run
 *             ....
 *             ....
 *         });
 *     }
 * \endcode
 */
class KPTY_EXPORT KPtyProcess : public KProcess
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(KPtyProcess)

public:
    /*!
     * \value NoChannels The PTY is not connected to any channel
     * \value StdinChannel Connect PTY to stdin
     * \value StdoutChannel Connect PTY to stdout
     * \value StderrChannel Connect PTY to stderr
     * \value AllOutputChannels Connect PTY to all output channels
     * \value AllChannels Connect PTY to all channels
     */
    enum PtyChannelFlag {
        NoChannels = 0,
        StdinChannel = 1,
        StdoutChannel = 2,
        StderrChannel = 4,
        AllOutputChannels = 6,
        AllChannels = 7,
    };
    Q_DECLARE_FLAGS(PtyChannels, PtyChannelFlag)

    /*!
     * Constructor
     */
    explicit KPtyProcess(QObject *parent = nullptr);

    /*!
     * Construct a process using an open pty master.
     *
     * \a ptyMasterFd an open pty master file descriptor.
     *   The process does not take ownership of the descriptor;
     *   it will not be automatically closed at any point.
     */
    KPtyProcess(int ptyMasterFd, QObject *parent = nullptr);

    ~KPtyProcess() override;

    /*!
     * Set to which channels the PTY should be assigned.
     *
     * This function must be called before starting the process.
     *
     * \a channels the output channel handling mode
     */
    void setPtyChannels(PtyChannels channels);

    /*!
     * Query to which channels the PTY is assigned.
     *
     * Returns the output channel handling mode
     */
    PtyChannels ptyChannels() const;

    /*!
     * Set whether to register the process as a TTY login in utmp.
     *
     * Utmp is disabled by default.
     * It should enabled for interactively fed processes, like terminal
     * emulations.
     *
     * This function must be called before starting the process.
     *
     * \a value whether to register in utmp.
     */
    void setUseUtmp(bool value);

    /*!
     * Get whether to register the process as a TTY login in utmp.
     *
     * Returns whether to register in utmp
     */
    bool isUseUtmp() const;

    /*!
     * Get the PTY device of this process.
     *
     * Returns the PTY device
     */
    KPtyDevice *pty() const;

private:
    std::unique_ptr<KPtyProcessPrivate> const d_ptr;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(KPtyProcess::PtyChannels)

#endif
