#include <sc-memory/sc_addr.hpp>
#include <sc-memory/sc_memory.hpp>
#include <iostream>
#include "find_articulation_point.hpp"


int main() 
{ 
    sc_memory_params params; 
    sc_memory_params_clear(&params); 
    params.repo_path = "/home/vaskho/Study/OSTIS/module_testing/kb.bin"; 
    params.config_file = "/home/vaskho/.config/OSTIS/sc-web.ini";
    params.ext_path = "/home/vaskho/.local/sc-machine/extensions"; 
    params.clear = SC_FALSE; 
    ScMemory mem; 

    mem.Initialize(params); 

    const std::unique_ptr<ScMemoryContext> context(new ScMemoryContext(sc_access_lvl_make_max, "example"));

    ScAddr graph, rrel_arcs, rrel_nodes;

    for (int i = 1; i <= 8; ++i)
    {
        graph = context->HelperResolveSystemIdtf("test" + std::to_string(i));
        std::cout << context->HelperGetSystemIdtf(graph) << std::endl;

        try
        {
            find_articulation_points(context, graph);
        }
        catch (std::string& error_message)
        {
            std::cout << error_message << std::endl;
            continue;
        }
    }

    std::cout << "articulation_point" << std::endl;

    ScIterator3Ptr art_point_iter = context->Iterator3(
            context->HelperResolveSystemIdtf("articulation_point"),
            ScType::EdgeAccessConstPosPerm,
            ScType::NodeConst
            );
    while(art_point_iter->Next()) std::cout << context->HelperGetSystemIdtf(art_point_iter->Get(2)) << std::endl;

    mem.Shutdown(true);
    return 0; 
}
