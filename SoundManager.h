#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

/// c++ standard
#include <string>
#include <map>
#include <vector>

using namespace std;

/// fmod
#include  "fmod/fmod.h"
#include "fmod/fmod_errors.h"

/// Grupo-T
#include "HashTable.h"
#include "Logger.h"

// SoundState enum
typedef enum
{
    /// El sonido no existe, o no se esta reproduciendo
    ST_NONE,

    /// El sonido esta en pleno play... lairalaira lairalaira lairala...lairailara
    ST_PLAYING,

    /// El sonido esta en pausa, ej. sonido del menu principal
    ST_PAUSED

}SoundState;


/*********************************
* Notar que la clase se basa en los nombre de archivo para manejar los sonidos
* Por lo tanto, hay restricciones a la hora de manejarlos
* 1- No se puede mandar a reproducir dos veces un archivo con repeat  = true
* 2- Creo que nada mas :)
**********************************/
/*********************************
* No creo que sea necesario, pero si sobra tiempo estaria bueno hacer streaming de sonido
* De esta forma, se podrian reproducir archivos pesados sin degradar la performance
* Un archivo de un mega y medio se lo banco bien cargarlo de prima
**********************************/
class SoundManager
{
public:
    void Play(char const *fileName, bool repeat = false, float volume = 0.5F);
    void Play(const string & fileName, bool repeat = false, float volume = 0.5F); // se puede usar path relativos

    virtual ~SoundManager();
    static SoundManager* getInstance();

    void PlayMusic(const string & fileName, bool repeat = false);
    void PlayGameSound(const string & fileName, bool repeat = false);

    void Stop(const string & fileName);

    void Pause(const string & fileName);

    void Resume(const string & fileName);

    bool isPlaying (const string & fileName);

    bool isPaused(const string & fileName);

    /*************************************************/
    /** Recomendado (mejor que isPaused, isPlaying) **/
    /*************************************************/
    SoundState GetSoundState(char const *fileName);
    SoundState GetSoundState(const string & fileName);

    void setMusicVolume(float volume);
    void setGameVolume(float volume);


    /***************************************************************/
    /*
    * Actualiza el sistema de sonido
    */
    void update();        /// IMPORTANTE!! -->> Llamarla en cada TIC !!
    /*
    */
    /***************************************************************/

private:
    static SoundManager * s_instance;
    SoundManager();

    float
        m_musicVol,
        m_gameVol;

    /// Inicializa el sistema de fmod
    bool Init();
    FMOD_SOUND* getSound(string);
};

#endif // SOUNDMANAGER_H
