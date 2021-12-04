#include <string>
#include "structures.h"

class NotAuthMode {
public:
    unauth_file_data_t add_unauth_user_file(const std::string &user_filename,
                                            const std::string &option_password);       // return unique id-code of file (success) or ""
    int delete_unauth_user_files(const std::string &upload_date);                       // return uuid (success or not)
    bool has_access_on_unauth_user_file(const std::string &file_uuid,
                                        const std::string &option_password);             // return success (1) or not (0)
};
