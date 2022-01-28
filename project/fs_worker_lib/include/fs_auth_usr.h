// Copyright 2021 nat-s.skv@mail.ru

#ifndef VVTI_FS_AUTH_USR_H
#define VVTI_FS_AUTH_USR_H

#include <string>
#include <filesystem>
#include <vector>
#include <ifs_entity.h>

class FsWorkerAuthUsr : public IFsWorkerEntity {
public:
    explicit FsWorkerAuthUsr(const std::filesystem::path &root_path);

    explicit FsWorkerAuthUsr(std::filesystem::path &&root_path);

    FsWorkerAuthUsr(const FsWorkerAuthUsr &src);

    FsWorkerAuthUsr(FsWorkerAuthUsr &&src) noexcept;

    ~FsWorkerAuthUsr() noexcept = default;

    bool move_root(const std::filesystem::path &new_root_path) noexcept override;

    bool move_file(const std::filesystem::path &src_path, const std::filesystem::path &dst_path, std::string user_id)  noexcept override;

    bool write_to_file(const std::string &file_content, const std::filesystem::path &dst_path,std::string user_id) noexcept;

    bool add(std::string user_id)  noexcept override;

    [[nodiscard]] std::ifstream get_file(const std::filesystem::path &file_path, std::string user_id)  noexcept override;

    bool remove(const std::filesystem::path &path, std::string user_id)  noexcept override;

    bool make_subdir(const std::filesystem::path &dir_path, std::string user_id)  noexcept override;

    [[nodiscard]] const std::filesystem::path &get_root() const noexcept override;

    bool operator==(const FsWorkerAuthUsr &rhs) const;

    std::error_code err_code;

private:
    std::filesystem::path root_path;
};

#endif //VVTI_FS_AUTH_USR_H
