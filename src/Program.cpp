#include "Program.hpp"
#include "Statement.hpp"
#include "utils/Error.hpp"

Program::Program() : programCounter_(-1), programEnd_(false) {}

void Program::addStmt(int line, std::unique_ptr<Statement> stmt) {
  recorder_.add(line, std::move(stmt));
}

void Program::removeStmt(int line) {
  recorder_.remove(line);
}

void Program::run() {
  resetAfterRun();
  programCounter_ = recorder_.nextLine(-1);
  while (programCounter_ != -1 && !programEnd_) {
    const Statement* stmt = recorder_.get(programCounter_);
    if (!stmt) {
      throw BasicError("LINE NUMBER ERROR");
    }
    int currentPC = programCounter_;
    stmt->execute(vars_, *this);
    if (programCounter_ == currentPC) {
      programCounter_ = recorder_.nextLine(programCounter_);
    }
  }
}

void Program::list() const {
  recorder_.printLines();
}

void Program::clear() {
  recorder_.clear();
  vars_.clear();
  resetAfterRun();
}

void Program::execute(Statement* stmt) {
  stmt->execute(vars_, *this);
}

int Program::getPC() const noexcept {
  return programCounter_;
}

void Program::changePC(int line) {
  if (!recorder_.hasLine(line)) {
    throw BasicError("LINE NUMBER ERROR");
  }
  programCounter_ = line;
}

void Program::programEnd() {
  programEnd_ = true;
}

void Program::resetAfterRun() noexcept {
  programCounter_ = -1;
  programEnd_ = false;
}