#ifndef BACKENDS_WINMM_H
#define BACKENDS_WINMM_H

#include "backends/base.h"

struct WinMMBackendFactory final : public BackendFactory {
public:
    bool init() override;

    bool querySupport(BackendType type) override;

    std::string probe(BackendType type) override;

    BackendPtr createBackend(ALCdevice *device, BackendType type) override;

    static BackendFactory &getFactory();
};

#endif /* BACKENDS_WINMM_H */
