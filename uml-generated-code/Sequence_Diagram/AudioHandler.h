
#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H

#include <string>

namespace Sequence_Diagram {


/**
  * class AudioHandler
  * 
  */

class AudioHandler
{
public:

  // Constructors/Destructors
  //  


  /**
   * Empty Constructor
   */
  AudioHandler ();

  /**
   * Empty Destructor
   */
  virtual ~AudioHandler ();

  // Static Public attributes
  //  

  // Public attributes
  //  


  // Public attribute accessor methods
  //  


  // Public attribute accessor methods
  //  



  /**
   * @return bool
   * @param  songPath
   */
  bool playSong (string songPath)
  {
  }


  /**
   * @return bool
   */
  bool isPlaying ()
  {
  }


  /**
   * @return bool
   */
  bool pausePlayback ()
  {
  }


  /**
   * @return bool
   */
  bool resumePlayback ()
  {
  }


  /**
   * @return bool
   * @param  newVolume
   */
  bool updateVolume (int newVolume)
  {
  }

protected:

  // Static Protected attributes
  //  

  // Protected attributes
  //  

public:


  // Protected attribute accessor methods
  //  

protected:

public:


  // Protected attribute accessor methods
  //  

protected:


private:

  // Static Private attributes
  //  

  // Private attributes
  //  

public:


  // Private attribute accessor methods
  //  

private:

public:


  // Private attribute accessor methods
  //  

private:



};
} // end of package namespace

#endif // AUDIOHANDLER_H
