/*
Copyright (c) 2017-2019, Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable Energy, LLC.  See the top-level NOTICE for additional details. All rights reserved.

SPDX-License-Identifier: BSD-3-Clause
*/

#include "helics/application_api/ValueFederate.hpp"
#include "helics/application_api/Inputs.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <regex>
#include <set>
#include <thread>
#include <stdexcept>
#include "helics/core/helicsCLI11.hpp"
#include "helics/core/helicsVersion.hpp"

int main (int argc, char *argv[])
{
    helics::helicsCLI11App app ("PubSub NS3 Example", "PubSubNS3Example");
    std::string myname = "fed";
    std::string publishKey = "ns3_test_value";

    app.add_option ("--name,-n", myname, "name of this federate");
    app.add_option ("--key,-k", publishKey, "name of the key to publish on");

    auto ret = app.helics_parse (argc, argv);

    helics::FederateInfo fi {};
    if (ret == helics::helicsCLI11App::parse_output::help_call)
    {
        fi.loadInfoFromArgs ("--help");
        return 0;
    }
    else if (ret != helics::helicsCLI11App::parse_output::ok)
    {
        return -1;
    }

    fi.loadInfoFromArgs (argc, argv);
    fi.setProperty (helics_property_int_log_level, 5);

    auto mFed = std::make_unique<helics::ValueFederate> (myname, fi);
    const auto name = mFed->getName();
    std::cout << " registering publication '" << publishKey << "' for " << name<<'\n';
    auto &pub = mFed->registerGlobalPublication<std::string> (publishKey);
    std::cout << " registering subscription '" << publishKey << "' for " << name<<'\n';
    auto &sub = mFed->registerSubscription (publishKey);

    std::cout << "entering init State\n";
    mFed->enterInitializingMode ();
    std::cout << "entered init State\n";
    mFed->enterExecutingMode ();
    std::cout << "entered exec State\n";
    for (int i=1; i<10; ++i)
    {
        const std::string message = "publishing message on "+publishKey+" at time " + std::to_string (i);
        mFed->publish (pub, message);
        std::cout << message << std::endl;
        const auto newTime = mFed->requestTime (i);
        std::cout << "processed time " << static_cast<double> (newTime) << "\n";
        while (sub.checkUpdate ())
        {
            const auto nmessage = sub.getValue<std::string> ();
            std::cout << "received publication on key " << sub.getKey () << " at " << static_cast<double> (sub.getLastUpdate ()) << " ::" << nmessage << '\n';
        }

    }
    mFed->finalize ();
    return 0;
}
