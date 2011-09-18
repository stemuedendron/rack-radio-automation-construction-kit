#ifndef RPLAYER_H
#define RPLAYER_H
#include "bass.h"

#include <QObject>
#include <QBasicTimer>

#include "bass.h"

/**
 * @brief
 *
 */
struct audiodevice
{
    int engineIndex;
    bool isInitialized;
    QString name;
    QString driver;
    QString userComment;
};

/**
 * @brief
 *
 */
class RPlayer : public QObject
{
    Q_OBJECT
    Q_ENUMS(State)

public:

    /**
     * @brief
     *
     */
    enum State
    {
        LoadingState,
        PlayingState,
        PausedState,
        FadingState,
        StoppedState,
        ErrorState,
        DisabledState
    };

    /**
     * @brief Create RPlayer object with the given device.
     *        You have to call RPlayer::init(int device) first.
     *        You can get a list of devices with RPlayer::getDevices()
     * @param parent
     */
    explicit RPlayer(QObject *parent = 0, const int &device = 0);

    /**
     * @brief
     *
     */
    ~RPlayer();

    /**
     * @brief
     *
     */
    State getState() const {return m_state;}


    /**
     * @brief
     *
     */
    static bool init(const int &device);

    /**
     * @brief
     *
     */
    static bool free(const int &device);

    /**
     * @brief
     *
     */
    static QList<audiodevice> getDevices();


public slots:

    /**
     * @brief
     *
     * @param int
     */
    void setDevice(const int &);

    /**
     * @brief
     *
     * @param float
     */
    void setVolume(const float &);

    /**
     * @brief
     *
     * @param DWORD
     */
    void setFadeInTime(const DWORD &);

    /**
     * @brief
     *
     * @param DWORD
     */
    void setFadeOutTime(const DWORD &);

    /**
     * @brief
     *
     * @param QString
     */
    void setURI(const QString &);

    /**
     * @brief
     *
     */
    void play();

    /**
     * @brief
     *
     */
    void pause();

    /**
     * @brief
     *
     */
    void stop();

    /**
     * @brief
     *
     */
    void fadeOut();

    /**
     * @brief
     *
     */
    void fadeIn();

    /**
     * @brief
     *
     */
    void playPause();

    /**
     * @brief
     *
     */
    void playStop();

    /**
     * @brief
     *
     */
    void playFadeOut();

    /**
     * @brief
     *
     */
    void fadeInStop();

    /**
     * @brief
     *
     */
    void fadeInPause();

    /**
     * @brief
     *
     */
    void fadeInFadeOut();

signals:

    /**
     * @brief This signal emmits the current position (in milliseconds) of the file currently being played.
     *
     * @param int time
     */
    void position(qint64 position);

    /**
     * @brief This signal emmits the total length (in milliseconds) of the file currently loaded.
     *        Must be connected before you call setURI.
     * @param int
     */
    void time(qint64 time);

    /**
     * @brief This signal is emitted when the player state has changed.
     *
     * @param
     */
    void stateChanged();

protected:

    /**
     * @brief
     *
     * @param event
     */
    void timerEvent(QTimerEvent *event);

private:

    int m_device;
    State m_state;
    HSTREAM m_stream; /**< TODO */
    float m_volume; /**< TODO */
    DWORD m_fadeInTime; /**< TODO */
    DWORD m_fadeOutTime; /**< TODO */
    bool m_isFadingOut; /**< TODO */
    QBasicTimer m_timer; /**< TODO */

};

#endif // RPLAYER_H
