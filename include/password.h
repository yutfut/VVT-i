//
// Created by Yut Fut on 18.12.2021.
//

#ifndef VVT_I_PASSWORD_H
#define VVT_I_PASSWORD_H

#include <iostream>
#include <unistd.h>

#include "validation.h"

std::string single_input_password();

std::string confirm_input_password();

std::string check_password_upload();

std::string check_password_download();

#endif //VVT_I_PASSWORD_H
