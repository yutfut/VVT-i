// Copyright 2021 nat-s.skv@mail.ru

#include <gtest/gtest.h>
#include <filesystem_lib.hpp>
#include <fstream>

FsNotAuthUsr create_fs_not_auth_usr(const fs::path &root_path) {
    FsNotAuthUsr fs_worker(root_path);
    EXPECT_EQ(root_path, fs_worker.get_root());
    EXPECT_FALSE(static_cast<bool>(fs_worker.err_code));
    EXPECT_TRUE(fs_worker.get_sort_dirs().empty());
    return fs_worker;
}

class FixtureCreateFsNotAuthUsrInvalid : public ::testing::Test {
protected:
    static void set_up(fs::path &&root_path, std::errc &&exp_errc) {
        FsNotAuthUsr fs_worker1(root_path);
        EXPECT_EQ(fs::path(""), fs_worker1.get_root());
        EXPECT_EQ(fs_worker1.err_code, exp_errc);
        EXPECT_TRUE(fs_worker1.get_sort_dirs().empty());

        fs::remove_all(root_path);
        FsNotAuthUsr fs_worker2(std::move(root_path));
        EXPECT_EQ(fs_worker1, fs_worker2);
    }
};

class FixtureFsNotAuthUsrMoveRootInvalid : public ::testing::Test {
protected:
    static void set_up(fs::path &root_path, const fs::path &new_root_path, std::errc &&exp_errc) {
        FsNotAuthUsr fs_worker(root_path);
        EXPECT_EQ(root_path, fs_worker.get_root());
        EXPECT_FALSE(static_cast<bool>(fs_worker.err_code));
        EXPECT_TRUE(fs_worker.get_sort_dirs().empty());

        fs_worker.move_root(new_root_path);
        EXPECT_EQ(fs_worker.err_code, exp_errc);
        EXPECT_EQ(fs_worker.get_root(), root_path);
        fs::remove_all(root_path);
    }
};

TEST(FsNotAuthUsr, CreateValid) {
    fs::path root_path = TEST_PATH / fs::path("input_output/fs_sub_worker_not_auth_usr/");
    FsNotAuthUsr fs_worker1(root_path);
    EXPECT_EQ(root_path, fs_worker1.get_root());
    EXPECT_FALSE(static_cast<bool>(fs_worker1.err_code));
    EXPECT_TRUE(fs_worker1.get_sort_dirs().empty());

    FsNotAuthUsr fs_worker2(fs_worker1);
    EXPECT_EQ(fs_worker2, fs_worker1);
    EXPECT_EQ(fs_worker2.err_code, std::errc(EEXIST));

    FsNotAuthUsr fs_worker3(std::move(fs_worker1));
    EXPECT_EQ(fs_worker3, fs_worker2);
    EXPECT_EQ(fs_worker3.err_code, std::errc(EEXIST));

    fs::remove_all(root_path);
    FsNotAuthUsr fs_worker4(std::move(root_path));
    EXPECT_EQ(fs_worker4, fs_worker2);
    EXPECT_FALSE(static_cast<bool>(fs_worker4.err_code));

    fs::remove_all(fs_worker4.get_root());
}

TEST_F(FixtureCreateFsNotAuthUsrInvalid, ENOENT) {
    fs::path root_path = TEST_PATH / fs::path("input_output/not_existing_dir/fs_sub_worker_not_auth_usr/");
    FixtureCreateFsNotAuthUsrInvalid::set_up(std::move(root_path), std::errc(ENOENT));
}

TEST_F(FixtureCreateFsNotAuthUsrInvalid, ENAMETOOLONG) {
    fs::path root_path = TEST_PATH "/input_output" / fs::path(std::string{_PC_NAME_MAX, 'x'});
    FixtureCreateFsNotAuthUsrInvalid::set_up(std::move(root_path), std::errc(ENAMETOOLONG));
}

TEST_F(FixtureCreateFsNotAuthUsrInvalid, ENOTDIR) {
    fs::path file_path = TEST_PATH "/input_output/file";
    std::ofstream file(file_path);
    file.close();
    ASSERT_TRUE(fs::exists(file_path));

    fs::path root_path = file_path / fs::path("fs_sub_worker_not_auth_usr/");
    FixtureCreateFsNotAuthUsrInvalid::set_up(std::move(root_path), std::errc(ENOTDIR));

    fs::remove(file_path);
}

TEST(FsNotAuthUsr, MoveRoot) {
    fs::path root_path = TEST_PATH / fs::path("input_output/fs_sub_worker_not_auth_usr/");
    FsNotAuthUsr fs_worker((root_path));
    fs::path new_root_path = TEST_PATH / fs::path("input_output/fs_sub_worker_not_auth_usr_new/");
    fs_worker.move_root(new_root_path);

    EXPECT_FALSE(static_cast<bool>(fs_worker.err_code));
    EXPECT_EQ(fs_worker.get_root(), new_root_path);
    std::error_code ec;
    EXPECT_TRUE(fs::exists(new_root_path, ec));
    EXPECT_FALSE(fs::exists(root_path, ec));

    fs::remove_all(new_root_path);
}

TEST_F(FixtureFsNotAuthUsrMoveRootInvalid, ENAMETOOLONG) {
    fs::path root_path = TEST_PATH / fs::path("input_output/fs_sub_worker_not_auth_usr/");
    fs::path new_root_path = TEST_PATH "/input_output" / fs::path(std::string{_PC_NAME_MAX, 'x'});
    FixtureFsNotAuthUsrMoveRootInvalid::set_up(root_path, new_root_path, std::errc(ENAMETOOLONG));
}

TEST_F(FixtureFsNotAuthUsrMoveRootInvalid, ENOENT) {
    fs::path root_path = TEST_PATH/ fs::path("input_output/fs_sub_worker_not_auth_usr/");
    fs::path new_root_path = TEST_PATH / fs::path("input_output/not_existing_dir/fs_sub_worker_not_auth_usr/");
    FixtureFsNotAuthUsrMoveRootInvalid::set_up(root_path, new_root_path, std::errc(ENOENT));
}

TEST_F(FixtureFsNotAuthUsrMoveRootInvalid, ENOTDIR) {
    fs::path root_path = TEST_PATH / fs::path("input_output/fs_sub_worker_not_auth_usr/");
    fs::path file_path = TEST_PATH "/input_output/file";
    std::ofstream file(file_path);
    file.close();
    ASSERT_TRUE(fs::exists(file_path));

    fs::path new_root_path = file_path / fs::path("fs_sub_worker_not_auth_usr/");
    FixtureFsNotAuthUsrMoveRootInvalid::set_up(root_path, new_root_path, std::errc(ENOTDIR));
    fs::remove(file_path);
}

TEST(FsNotAuthUsr, CreateDayDirAndRemoveExpired) {
    fs::path root_path = TEST_PATH / fs::path("input_output/fs_sub_worker_not_auth_usr/");
    FsNotAuthUsr fs_worker((root_path));
    EXPECT_TRUE(fs_worker.get_sort_dirs().empty());
    ASSERT_FALSE(static_cast<bool>(fs_worker.err_code));
    std::vector<fs::path> dates = {"10-10-2020", "11-10-2020", "12-10-2020"};

    std::for_each(dates.begin(), dates.end(),
                  [&fs_worker](const auto &date) {
                      fs_worker.reset_error_code();
                      EXPECT_TRUE(fs_worker.create_day_dir(static_cast<std::string>(date)));
                      EXPECT_FALSE(static_cast<bool>(fs_worker.err_code));
                      std::error_code ec;
                      EXPECT_TRUE(fs::exists(fs_worker.get_root() / date, ec));
                  });

    auto sort_dirs = fs_worker.get_sort_dirs();
    EXPECT_EQ(dates, sort_dirs);

    for (size_t i = sort_dirs.size(); i > 0ul; --i) {
        fs_worker.reset_error_code();
        fs_worker.set_file_expiration_date(i);
        EXPECT_TRUE(fs_worker.remove_expired_dirs());

        std::for_each(dates.begin(), dates.begin() + static_cast<int>(i),
                      [&root_path](const auto &dir) {
                          std::error_code ec;
                          EXPECT_TRUE(fs::exists(root_path / dir, ec));
                      });
        std::for_each(dates.begin() + static_cast<int>(i), dates.end(),
                      [&root_path](const auto &dir) {
                          std::error_code ec;
                          EXPECT_FALSE(fs::exists(root_path / dir, ec));
                      });
    }

    fs::remove_all(root_path);
}

TEST(FsNotAuthUsr, MoveAndGetFile) {
    fs::path root_path = TEST_PATH/ fs::path("input_output/fs_sub_worker_not_auth_usr/");
    FsNotAuthUsr fs_worker((root_path));
    EXPECT_TRUE(fs_worker.get_sort_dirs().empty());
    ASSERT_FALSE(static_cast<bool>(fs_worker.err_code));

    std::vector<fs::path> dates = {"10-10-2020", "11-10-2020", "12-10-2020"};

    std::for_each(dates.begin(), dates.end(),
                  [&fs_worker](const auto &date) {
                      fs_worker.reset_error_code();
                      EXPECT_TRUE(fs_worker.create_day_dir(static_cast<std::string>(date)));
                      EXPECT_FALSE(static_cast<bool>(fs_worker.err_code));
                      std::error_code ec;
                      EXPECT_TRUE(fs::exists(fs_worker.get_root() / date, ec));
                  });

    auto sort_dirs = fs_worker.get_sort_dirs();
    EXPECT_EQ(dates, sort_dirs);

    fs::path file_path = root_path / fs::path("test_move");
    std::ofstream file(file_path);
    ASSERT_TRUE(file.is_open());
    file << "This is test of FsNotAuthUsr::move_file()";
    file.close();

    fs::path file_hlink_path = root_path / fs::path("test_move_hlink");
    for (const auto &date_dir: sort_dirs) {
        std::error_code ec;
        fs::create_hard_link(file_path, file_hlink_path, ec);
        if (static_cast<bool>(ec)) {
            EXPECT_FALSE("creation of hard link failed. Part of the test skipped.\n");
            continue;
        }
        fs_worker.reset_error_code();
        fs::path fname_moved("moved_file");
        EXPECT_TRUE(fs_worker.move_file_to_fs(file_hlink_path, fname_moved, date_dir));

        fs::path moved_file_path = root_path / date_dir / fname_moved;
        EXPECT_TRUE(fs::equivalent(file_path, moved_file_path, ec));
        EXPECT_TRUE(fs_worker.get_file(fname_moved, date_dir).is_open());
    }

    fs::remove_all(root_path);
}
