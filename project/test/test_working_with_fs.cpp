// Copyright 2021 nat-s.skv@mail.ru

#include <gtest/gtest.h>
#include <working_with_filesystem.hpp>
#include <fstream>

class FixtureCreateFsWorker : public ::testing::Test {
protected:
    void SetUp(fs::path &&rootPath) {
        FsWorker fsWorker1(rootPath);
        FsWorker fsWorker2(std::move(rootPath));
    }
};

TEST_F(FixtureUploadFile, CreateFsWorker) {
     FixtureUploadFile::SetUp(fs::path(TEST_PATH "/"));
}

TEST(CreateFsWorker, CreateFromLValueLinks) {
    const fs::path rootPath=TEST_PATH;
    FsWorker fsWorker()
}
