#pragma once

enum FileStatus
{
        creating,
        downloading,
        verification,
        downloaded,
        distribution,
        failing,
        pause,
        deleting,
        outgoing,
        incoming,
};
