/*
            CVT - Computer Vision Tools Library

     Copyright (c) 2012, Philipp Heise, Sebastian Klose

    THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
    PARTICULAR PURPOSE.
 */

#include <cvt/io/ChameleonStereo.h>

#define PG_STROBE_PRESENCE_BIT ( 1 << 31 )
#define PG_STROBE_ON_OFF_BIT ( 1 << 25 )
#define PG_SIG_POLARITY_BIT ( 1 << 24 )

namespace cvt {

    ChameleonStereo::ChameleonStereo( const Parameters &params ) :
        StereoInput(),
        _leftCam( 0 ),
        _rightCam( 0 )
    {
        // init cams
        size_t n = DC1394Camera::count();
        if( n < 2 ){
            throw CVTException( "not enough cameras connected" );
        }

        size_t leftIdx = idxForId( params.leftId );
        size_t rightIdx = idxForId( params.rightId );

        DC1394Camera::Parameters camParams;
        camParams.isoSpeed = 400;
        camParams.numDMABuf = 4;
        camParams.usePreset = true;
        camParams.preset = DC1394Camera::PRESET_USER0;

        // leftcam should run continuous
        camParams.runMode = DC1394Camera::RUNMODE_CONTINUOUS;
        CameraInfo cInfo;
        DC1394Camera::cameraInfo( leftIdx, cInfo );
        CameraMode mode = cInfo.bestMatchingMode( IFormat::BAYER_GBRG_UINT8, 1296, 960, 15 );

        std::cout << mode << std::endl;
        mode.fps = 16;

        _leftCam = new DC1394Camera( leftIdx, mode, camParams );

        camParams.runMode = DC1394Camera::RUNMODE_HW_TRIGGER;
        _rightCam = new DC1394Camera( rightIdx, mode, camParams );

        //configureLeft( params.leftStrobePin );
        //configureRight( params.rightTriggerPin );

        _leftCam->startCapture();
        _rightCam->startCapture();
    }

    ChameleonStereo::~ChameleonStereo()
    {
        if( _leftCam ){
            _leftCam->stopCapture();
            delete _leftCam;
        }
        if( _rightCam ){
            _rightCam->stopCapture();
            delete _rightCam;
        }
    }

    bool ChameleonStereo::nextFrame( size_t timeout )
    {
        if( _leftCam->nextFrame( timeout ) ){
            _leftRect   = _leftCam->frame();
            if( _rightCam->nextFrame( timeout ) ){
                // TODO: undistort rectify with calibration data
                _rightRect  = _rightCam->frame();
                return true;
            }
        }
        return false;
    }

    size_t ChameleonStereo::idxForId( const cvt::String& id ) const
    {
        size_t n = DC1394Camera::count();
        CameraInfo info;

        for( size_t i = 0; i < n; ++i ){
            DC1394Camera::cameraInfo( i, info );
            if( info.identifier() == id ){
                // get this camera
                return info.index();
            }
        }
        throw CVTException( "Could not find requested camera id" );
        return 0;
    }

    void ChameleonStereo::configureLeft( int strobePin )
    {
        // set to asynchronous triggering
        _leftCam->startCapture();
        _leftCam->setRunMode( cvt::DC1394Camera::RUNMODE_SW_TRIGGER );
        _leftCam->enableExternalTrigger( true );
        _leftCam->setExternalTriggerMode( cvt::DC1394Camera::EDGE_TRIGGERED_FIXED_EXPOSURE );
        // trigger from SW
        _leftCam->setTriggerSource( cvt::DC1394Camera::TRIGGER_SOURCE_SOFTWARE );
        configureStrobe( strobePin );
    }

    void ChameleonStereo::configureStrobe( int pin )
    {
        // configure strobePin to output strobe, as long as exposure time
        uint64_t baseAddress = _leftCam->commandRegistersBase();
        static const uint64_t StrobeOutputInq = 0x48C;
        uint32_t strobeAddress = _leftCam->getRegister( baseAddress + StrobeOutputInq );

        strobeAddress = ( strobeAddress << 2 ) & 0xFFFFF;

        uint32_t strobeCtrlInq = _leftCam->getRegister( baseAddress + strobeAddress );

        uint32_t pinOffset = strobeAddress + 0x200 + 4 * pin;

        if( pin > 3 || pin < 0 ){
            throw CVTException( "unknown pin!" );
        }

        if( !( strobeCtrlInq & ( 1 << ( 31 - pin ) ) ) ){
            throw CVTException( "Strobe not present for requested pin" );
        }

        uint32_t strobeReg = _leftCam->getRegister( baseAddress + pinOffset );

        // stop strobe when streaming stops
        strobeReg |= PG_STROBE_ON_OFF_BIT;

        // trigger on rising edge
        strobeReg |= PG_SIG_POLARITY_BIT;
        //strobeReg &= ~PG_SIG_POLARITY_BIT;

        // no delay, strobe length = expose length
        strobeReg &= 0xFF000000;
        _leftCam->setRegister( baseAddress + pinOffset, strobeReg );
    }

    void ChameleonStereo::configureRight( int triggerPin )
    {
        // slave shall trigger asynchronous
        _rightCam->startCapture();
        _rightCam->setRunMode( cvt::DC1394Camera::RUNMODE_HW_TRIGGER );
        _rightCam->enableExternalTrigger( true );
        _rightCam->setExternalTriggerMode( cvt::DC1394Camera::EDGE_TRIGGERED_EDGE_EXPOSURE );
        _rightCam->setExternalTriggerPolarity( cvt::DC1394Camera::TRIGGER_ON_RISING_EDGE );

        // configure to capture frame on signal change on triggerpin
        cvt::DC1394Camera::ExternalTriggerSource triggerSource = cvt::DC1394Camera::TRIGGER_SOURCE_0;
        switch( triggerPin ){
            case 0: triggerSource = cvt::DC1394Camera::TRIGGER_SOURCE_0; break;
            case 1: triggerSource = cvt::DC1394Camera::TRIGGER_SOURCE_1; break;
            case 2: triggerSource = cvt::DC1394Camera::TRIGGER_SOURCE_2; break;
            case 3: triggerSource = cvt::DC1394Camera::TRIGGER_SOURCE_3; break;
            default:
                throw CVTException( "unknown pin number for trigger source" );
        }
        _rightCam->setTriggerSource( triggerSource );
    }

    void ChameleonStereo::setShutter( float val )
    {
        _leftCam->setShutterAbs( val );
        _rightCam->setShutterAbs( val );
    }

    void ChameleonStereo::setGain( float val )
    {
        _leftCam->setGainAbs( val );
        _rightCam->setGainAbs( val );
    }

    void ChameleonStereo::setAutoShutter( bool val )
    {
        DC1394Camera::FeatureMode mode = DC1394Camera::MANUAL;
        if( val ){
            mode = DC1394Camera::AUTO;
        }
        _leftCam->setShutterMode( mode );
        _rightCam->setShutterMode( mode );
    }

    void ChameleonStereo::setAutoGain( bool val )
    {
        DC1394Camera::FeatureMode mode = DC1394Camera::MANUAL;
        if( val ){
            mode = DC1394Camera::AUTO;
        }
        _leftCam->setShutterMode( mode );
        _rightCam->setShutterMode( mode );
    }

    void ChameleonStereo::setAutoExposure( bool val )
    {
        DC1394Camera::FeatureMode mode = DC1394Camera::MANUAL;
        if( val ){
            mode = DC1394Camera::AUTO;
        }
        _leftCam->setExposureMode( mode );
        _rightCam->setExposureMode( mode );
    }

    void ChameleonStereo::enableAutoExposure( bool val )
    {
        _leftCam->enableAutoExposure( val );
        _rightCam->enableAutoExposure( val );
    }

    void ChameleonStereo::setExposure( float val )
    {
        _leftCam->setExposureValueAbs( val );
        _rightCam->setExposureValueAbs( val );
    }
}