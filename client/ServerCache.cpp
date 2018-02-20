// Copyright (c) 2018-2018 Josh Blum
// SPDX-License-Identifier: BSL-1.0

#include "SoapyClient.hpp"
#include "SoapySSDPEndpoint.hpp"
#include "SoapyMDNSEndpoint.hpp"
#include "SoapyRemoteDefs.hpp"
#include <memory>
#include <future>

std::vector<std::string> SoapyRemoteDevice::getServerURLs(const int ipVer, const long timeoutUs)
{
    //connect to DNS-SD daemon and maintain a global connection
    //logic will reconnect if the status has failed for some reason
    static std::unique_ptr<SoapyMDNSEndpoint> mdnsEndpoint(new SoapyMDNSEndpoint());
    if (not mdnsEndpoint->status()) mdnsEndpoint.reset(new SoapyMDNSEndpoint());

    //On non-windows platforms the endpoint instance can last the
    //duration of the process because it can be cleaned up safely.
    //Windows has issues cleaning up threads and sockets on exit.
    #ifndef _MSC_VER
    static
    #endif //_MSC_VER
    std::unique_ptr<SoapySSDPEndpoint> ssdpEndpoint(new SoapySSDPEndpoint());

    //get all IPv4 and IPv6 URLs because we will fallback
    //to the other protocol if the server was found,
    //but not under the preferred IP protocol version.
    auto mdnsUrls = std::async(std::launch::async, &SoapyMDNSEndpoint::getServerURLs, mdnsEndpoint.get(), SOAPY_REMOTE_IPVER_UNSPEC, timeoutUs);
    auto ssdpUrls = std::async(std::launch::async, &SoapySSDPEndpoint::getServerURLs, ssdpEndpoint.get(), SOAPY_REMOTE_IPVER_UNSPEC, timeoutUs);

    //merge the results from the two discovery protocols
    auto uuidToUrl = ssdpUrls.get();
    for (const auto &uuidToMap : mdnsUrls.get())
    {
        for (const auto &verToUrl : uuidToMap.second)
        {
            uuidToUrl[uuidToMap.first][verToUrl.first] = verToUrl.second;
        }
    }

    //select the URL according to the ipVersion preference
    std::vector<std::string> serverUrls;
    for (const auto &uuidToMap : uuidToUrl)
    {
        //prefer version match when found
        auto itVer = uuidToMap.second.find(ipVer);
        if (itVer != uuidToMap.second.end())
        {
            serverUrls.push_back(itVer->second);
        }

        //otherwise fall-back to any discovery
        else if (not uuidToMap.second.empty())
        {
            serverUrls.push_back(uuidToMap.second.begin()->second);
        }
    }
    return serverUrls;
}
