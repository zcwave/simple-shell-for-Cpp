#include <iostream>
#include "Jobs.hpp"

bool verbose = true;
// 测试添加作业
void testAddJob() {
    Jobs& jobs = Jobs::getInstance();
    jobs.addJob(1234, FG, "ls -l");
    // 还可以添加更多测试逻辑，例如检查添加的作业是否在作业列表中等
}

// 测试删除作业
void testDeleteJob() {
    Jobs& jobs = Jobs::getInstance();
    jobs.deleteJob(1234);
    // 还可以添加更多测试逻辑，例如检查删除后作业列表是否为空等
}

// 测试获取前台作业的 PID
void testGetFgPid() {
    Jobs& jobs = Jobs::getInstance();
    pid_t fgpid = jobs.getFgPid();
    // 还可以添加更多测试逻辑，例如检查是否返回了正确的前台作业 PID 等
}

// 测试根据 PID 查找作业
void testGetJobByPid() {
    Jobs& jobs = Jobs::getInstance();
    std::optional<job_t> jobByPid = jobs.getJobByPid(1234);
    // 还可以添加更多测试逻辑，例如检查是否返回了正确的作业信息等
}

// 测试根据 JID 查找作业
void testGetJobByJid() {
    Jobs& jobs = Jobs::getInstance();
    std::optional<job_t> jobByJid = jobs.getJobByJid(1);
    // 还可以添加更多测试逻辑，例如检查是否返回了正确的作业信息等
}

// 测试根据 PID 查找对应的 JID
void testPid2Jid() {
    Jobs& jobs = Jobs::getInstance();
    std::optional<int> jid = jobs.pid2jid(1234);
    // 还可以添加更多测试逻辑，例如检查是否返回了正确的 JID 等
}

// 测试打印作业列表
void testListJobs() {
    Jobs& jobs = Jobs::getInstance();
    jobs.addJob(1111, BG, "hello");
    jobs.addJob(1111, BG, "hello");
    jobs.addJob(1111, BG, "hello");
    jobs.addJob(1111, BG, "hello");
    jobs.addJob(1111, BG, "hello");
    jobs.addJob(1111, BG, "hello");
    jobs.list();
    // 还可以添加更多测试逻辑，例如检查打印的作业列表是否正确等
}

// 主测试函数
int main() {
    testAddJob();
    testDeleteJob();
    testGetFgPid();
    testGetJobByPid();
    testGetJobByJid();
    testPid2Jid();
    testListJobs();
    return 0;
}
