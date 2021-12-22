// Copyright 2021 nat-s.skv@mail.ru

#ifndef VVTI_FS_AUTH_USR_H
#define VVTI_FS_AUTH_USR_H

#include <string>
#include <filesystem>
#include <vector>
#include <ifs_entity.h>

namespace fs = std::filesystem;

class FsWorkerAuthUsr : public IFsWorkerEntity {
public:
    explicit FsWorkerAuthUsr(const fs::path &user_dir);

    explicit FsWorkerAuthUsr(fs::path &&user_dir);

    FsWorkerAuthUsr(const FsWorkerAuthUsr &src);

    FsWorkerAuthUsr(FsWorkerAuthUsr &&src) noexcept;

    ~FsWorkerAuthUsr() noexcept = default;

    bool move_root(const fs::path &new_user_dir) noexcept override;

    bool move_file(const fs::path &src_path, const fs::path &dst_path, std::string user_id) const noexcept override;

    bool add(std::string user_id) const noexcept override;

    [[nodiscard]] std::ifstream get_file(const fs::path &file_path, std::string user_id) const noexcept override;

    bool remove(const fs::path &path, std::string user_id) const noexcept override;

    bool make_subdir(const fs::path &dir_path, std::string user_id) const noexcept override;

    [[nodiscard]] const fs::path &get_root() const noexcept override;

    void reset_error_code() noexcept override;

    bool operator==(const FsWorkerAuthUsr &rhs) const;

    std::error_code err_code;

private:
    fs::path root_path;
};

#endif //VVTI_FS_AUTH_USR_H
