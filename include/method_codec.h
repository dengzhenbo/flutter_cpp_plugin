#ifndef CPP_PLUGIN_METHOD_CODEC_H_
#define CPP_PLUGIN_METHOD_CODEC_H_

#include <memory>
#include <string>
#include <vector>

#include "method_call.h"

namespace cpp_plugin {

// Translates between a binary message and higher-level method call and
// response/error objects.
class MethodCodec {
 public:
  virtual ~MethodCodec();

  // Returns the MethodCall encoded in |message|, or nullptr if it cannot be
  // decoded.
  // TODO: Consider adding absl as a dependency and using absl::Span.
  std::unique_ptr<MethodCall> DecodeMethodCall(const uint8_t *message,
                                               const size_t message_size) const;

  // Returns a binary encoding of the given |method_call|, or nullptr if the
  // method call cannot be serialized by this codec.
  std::unique_ptr<std::vector<uint8_t>> EncodeMethodCall(
      const MethodCall &method_call) const;

  // Returns a binary encoding of |result|. |result| must be a type supported
  // by the codec.
  std::unique_ptr<std::vector<uint8_t>> EncodeSuccessEnvelope(
      const void *result = nullptr) const;

  // Returns a binary encoding of |error|. The |error_details| must be a type
  // supported by the codec.
  std::unique_ptr<std::vector<uint8_t>> EncodeErrorEnvelope(
      const std::string &error_code, const std::string &error_message = "",
      const void *error_details = nullptr) const;

 protected:
  friend class PluginManager;

  // Implementations of the public interface, to be provided by subclasses.
  virtual std::unique_ptr<MethodCall> DecodeMethodCallInternal(
      const uint8_t *message, const size_t message_size) const = 0;
  virtual std::unique_ptr<std::vector<uint8_t>> EncodeMethodCallInternal(
      const MethodCall &method_call) const = 0;
  virtual std::unique_ptr<std::vector<uint8_t>> EncodeSuccessEnvelopeInternal(
      const void *result) const = 0;
  virtual std::unique_ptr<std::vector<uint8_t>> EncodeErrorEnvelopeInternal(
      const std::string &error_code, const std::string &error_message,
      const void *error_details) const = 0;
};

}  // namespace cpp_plugin

#endif  // CPP_PLUGIN_METHOD_CODEC_H_
