// Copyright (c) 2022 by Apex.AI Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#ifndef IOX_POSH_POPO_SERVER_IMPL_HPP
#define IOX_POSH_POPO_SERVER_IMPL_HPP

#include "iceoryx_posh/capro/service_description.hpp"
#include "iceoryx_posh/internal/popo/base_server.hpp"
#include "iceoryx_posh/internal/popo/request_deleter.hpp"
#include "iceoryx_posh/internal/popo/response_deleter.hpp"
#include "iceoryx_posh/internal/popo/rpc_interface.hpp"
#include "iceoryx_posh/popo/request.hpp"
#include "iceoryx_posh/popo/response.hpp"
#include "iceoryx_posh/popo/server_options.hpp"
#include "iceoryx_posh/popo/trigger_handle.hpp"
#include "iceoryx_posh/runtime/posh_runtime.hpp"

namespace iox
{
namespace popo
{
template <typename Req, typename Res, typename BaseServerT = BaseServer<>>
class ServerImpl : public BaseServerT, public RpcInterface<Response<Res>>
{
  public:
    explicit ServerImpl(const capro::ServiceDescription& service, const ServerOptions& serverOptions = {}) noexcept;

    cxx::expected<Request<const Req>, ServerRequestResult> take() noexcept;

    template <typename... Args>
    cxx::expected<Response<Res>, AllocationError> loan(const Request<const Req>& request, Args&&... args) noexcept;

    void send(Response<Res>&& response) noexcept override;

  private:
    using BaseServerT::port;

    cxx::expected<Response<Res>, AllocationError> loanUninitialized(const Request<const Req>& request) noexcept;

    using RequestSampleDeleter = RequestDeleter<typename BaseServerT::PortType>;
    RequestSampleDeleter m_requestDeleter{port()};
    using ResponseSampleDeleter = ResponseDeleter<typename BaseServerT::PortType>;
    ResponseSampleDeleter m_responseDeleter{port()};
};
} // namespace popo
} // namespace iox

#include "iceoryx_posh/internal/popo/server_impl.inl"

#endif // IOX_POSH_POPO_SERVER_IMPL_HPP
