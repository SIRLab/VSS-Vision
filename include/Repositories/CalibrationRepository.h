/*
 * This file is part of the VSS-Vision project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#ifndef CALIBRATION_REPOSITORY_H
#define CALIBRATION_REPOSITORY_H

#include <Domain/ConfigurationType.h>
#include <Domain/ColorType.h>
#include "ICalibrationRepository.h"
#include "Calibration.h"

class CalibrationRepository : public ICalibrationRepository {
private:
    ColorType hasColorType(std::string name);
    ConfigurationType hasConfigurationType(std::string name);

public:
    Calibration read(std::string pathName) override;
    Calibration update(std::string pathName, Calibration calibration) override;
    void remove(std::string pathName) override;
    void create(std::string pathName, Calibration calibration) override;
};

#endif // CALIBRATION_REPOSITORY_H
