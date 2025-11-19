#pragma once

namespace Params
{
    enum class TempoSyncTimes
    {
        tempoSyncOff,
        whole,
        dottedHalf,
        half,
        tripletHalf,
        dottedQuarter,
        quarter,
        tripletQuarter,
        dottedEighth,
        eighth,
        tripletEighth,
        dottedSixteenth,
        sixteenth,
        tripletSixteenth,
        dottedThirtysecond,
        thirtysecond,
        tripletThirtysecond,
        dottedSixtyforth,
        sixtyforth,
        tripletSixtyforth,
        numTempoSyncTimes
    };

    enum class PanModes
    {
        stationary,
        pingPong,
        circular,
        numPanModes
    };
}

