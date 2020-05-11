#include "cinn/backends/extern_func_emitter.h"
#include "cinn/backends/llvm/runtime_symbol_registry.h"

#include <functional>
#include <string>

#include "cinn/backends/extern_func_emitter_builtin.h"

namespace cinn {
namespace backends {

ExternFunctionEmitterRegistry& ExternFunctionEmitterRegistry::Global() {
  static ExternFunctionEmitterRegistry x;
  return x;
}

void ExternFunctionEmitterRegistry::Register(const ExternFuncID& name, ExternFuncEmitter* x) {
  LOG(WARNING) << "Register extern function emitter [" << name << "]";
  CHECK(x);
  data_[name] = std::unique_ptr<ExternFuncEmitter>(x);
}

ExternFuncEmitter* ExternFunctionEmitterRegistry::Lookup(const ExternFuncID& name) const {
  auto it = data_.find(name);
  if (it != data_.end()) {
    return it->second.get();
  }
  return nullptr;
}

std::ostream& operator<<(std::ostream& os, const ExternFuncID& x) {
  os << x.name << ":" << x.backend_id;
  return os;
}

ExternFunctionEmitterRegistry::ExternFunctionEmitterRegistry() {
  Register(ExternFuncID(backend_C, extern_func__tanh), new ExternFuncEmitter_C_tanh);
  Register(ExternFuncID(backend_llvm_host, extern_func__tanh), new ExternFuncEmitter_LLVM_tanh);

  // Register the runtime functions.
  RuntimeSymbolRegistry::Global().Register(extern_tanh_host_repr, reinterpret_cast<void*>(__cinn_host_tanh));
}

const FunctionProto& ExternFuncEmitter::func_proto() const {
  auto* proto = ExternFunctionProtoRegistry::Global().Lookup(func_name());
  CHECK(proto) << "No prototype of function [" << func_name() << "]";
  return *proto;
}

}  // namespace backends
}  // namespace cinn

namespace std {

size_t hash<cinn::backends::ExternFuncID>::operator()(const cinn::backends::ExternFuncID& x) const {
  return std::hash<std::string_view>{}(x.name) ^ std::hash<std::string_view>{}(x.backend_id);
}

}  // namespace std