#include "find_articulation_point.hpp"


void dfs(const std::unique_ptr<ScMemoryContext>&, ScAddr, ScAddr, int=1);

void find_articulation_points(const std::unique_ptr<ScMemoryContext>& context, ScAddr graph)
{
    ScAddr articulation_point = context->HelperResolveSystemIdtf("articulation_point");
    ScAddr nodes(ScType::Unknown), arcs(ScType::Unknown);

    // Iterator to get node which includes all graph nodes
    ScIterator5Ptr nodes_iter = context->Iterator5(
            graph,
            ScType::EdgeAccessConstPosPerm,
            ScType::Unknown,
            ScType::EdgeAccessConstPosPerm,
            context->HelperResolveSystemIdtf("rrel_nodes")
            );
    if (nodes_iter->Next()) nodes = nodes_iter->Get(2);
    else throw("Unknown construction");

    // Iterator to get 'nodes -> vertex'
    ScIterator3Ptr vertexes = context->Iterator3(
            nodes,
            ScType::EdgeAccessConstPosPerm,
            ScType::Unknown
            );

    // Now we create all nessessary nodes in memory, i.e.
    // 1. _tin <- sc_node_norole_relation;;
    ScAddr nrel_tin = context->CreateNode(ScType::NodeVarNoRole);
    context->HelperSetSystemIdtf("_tin", nrel_tin);
    // 2. _tup <- sc_node_norole_relation;;
    ScAddr nrel_tup = context->CreateNode(ScType::NodeVarNoRole);
    context->HelperSetSystemIdtf("_tup", nrel_tup);
    // 3. _visited_vertexes <- sc_node;;
    ScAddr visited_vertexes = context->CreateNode(ScType::NodeVar);
    context->HelperSetSystemIdtf("_visited_vertexes", visited_vertexes);
    // 4. _concept_timer <- sc_node_class;;
    ScAddr timer_class = context->CreateNode(ScType::NodeVarClass);
    context->HelperSetSystemIdtf("_concept_timer", timer_class);

    // Now 'vertexes' is ScNode with all vertexes of specified graph
    
    if(vertexes->Next())
    {
        ScAddr current_vertex = vertexes->Get(2);
        dfs(context, current_vertex, current_vertex);
    }
}


int min(const std::unique_ptr<ScMemoryContext>& context, ScAddr a, ScAddr b)
{
    int a_value = std::stoi(context->HelperGetSystemIdtf(a));
    int b_value = std::stoi(context->HelperGetSystemIdtf(b));
    return (a_value < b_value)? a_value: b_value;
}


void set_tin(const std::unique_ptr<ScMemoryContext>& context, ScAddr node, int tin)
{
    // Iterator to get '_concept_timer _-> tin;;'
    ScIterator3Ptr timer_values = context->Iterator3(
            context->HelperResolveSystemIdtf("_concept_timer"),
            ScType::EdgeAccessVarPosPerm,
            context->HelperResolveSystemIdtf(std::to_string(tin))
            );
    // We check whether tin node already exists in memory
    if (timer_values->Next())
    {
        // Iterator to find 'node _=> _tin: str(tin);;'
        ScIterator5Ptr edges_search = context->Iterator5(
                node,
                ScType::EdgeDCommonVar,
                context->HelperResolveSystemIdtf(std::to_string(tin)),
                ScType::EdgeAccessVarPosPerm,
                context->HelperResolveSystemIdtf("_tin")
                );
        if (edges_search->Next())
        {
            // Here we erase edges '_tin _-> (node _-> str(tin));;'
            // node _-> str(tin);;
            context->EraseElement(edges_search->Get(3));
            context->EraseElement(edges_search->Get(1));
        }
        // Create new edge 'node _=> str(tin);;'
        ScAddr tin_edge = context->CreateEdge(ScType::EdgeDCommonVar, node, timer_values->Get(2));
        // Configure edge between _tin relation
        context->CreateEdge(ScType::EdgeAccessVarPosPerm, context->HelperResolveSystemIdtf("_tin"), tin_edge);
    }
    else
    {
        // If no, we create new node and add nessessary edges
        ScAddr new_tin = context->CreateNode(ScType::NodeVar);
        context->HelperSetSystemIdtf(std::to_string(tin), new_tin);
        // Here we make 5-elem construction 'node _=> _tin: str(tin);;'
        ScAddr tin_edge = context->CreateEdge(ScType::EdgeDCommonVar, node, new_tin);
        context->CreateEdge(ScType::EdgeAccessVarPosPerm, context->HelperResolveSystemIdtf("_tin"), tin_edge);
        // Add node to _concept_timer
        context->CreateEdge(ScType::EdgeAccessVarPosPerm, context->HelperResolveSystemIdtf("_concept_timer"), new_tin);
    }
}


void set_tup(const std::unique_ptr<ScMemoryContext>& context, ScAddr node, int tup)
{
    // Iterator to get '_concept_timer _-> tup;;'
    ScIterator3Ptr timer_values = context->Iterator3(
            context->HelperResolveSystemIdtf("_concept_timer"),
            ScType::EdgeAccessVarPosPerm,
            context->HelperResolveSystemIdtf(std::to_string(tup))
            );
    // We check whether tup node already exists in memory
    if (timer_values->Next())
    {
        // Iterator to find 'node _=> _tup: str(tup);;'
        ScIterator5Ptr edges_search = context->Iterator5(
                node,
                ScType::EdgeDCommonVar,
                context->HelperResolveSystemIdtf(std::to_string(tup)),
                ScType::EdgeAccessVarPosPerm,
                context->HelperResolveSystemIdtf("_tup")
                );
        if (edges_search->Next())
        {
            // Here we erase edges '_tup _-> (node _-> str(tup));;'
            // node _-> str(tup);;
            context->EraseElement(edges_search->Get(3));
            context->EraseElement(edges_search->Get(1));
        }
        // Create new edge 'node _=> str(tup);;'
        ScAddr tup_edge = context->CreateEdge(ScType::EdgeDCommonVar, node, timer_values->Get(2));
        // Configure edge between _tup relation
        context->CreateEdge(ScType::EdgeAccessVarPosPerm, context->HelperResolveSystemIdtf("_tup"), tup_edge);
    }
    else
    {
        // If no, we create new node and add nessessary edges
        ScAddr new_tup = context->CreateNode(ScType::NodeVar);
        context->HelperSetSystemIdtf(std::to_string(tup), new_tup);
        // Here we make 5-elem construction 'node _=> _tup: str(tup);;'
        ScAddr tup_edge = context->CreateEdge(ScType::EdgeDCommonVar, node, new_tup);
        context->CreateEdge(ScType::EdgeAccessVarPosPerm, context->HelperResolveSystemIdtf("_tup"), tup_edge);
        // Add node to _concept_timer
        context->CreateEdge(ScType::EdgeAccessVarPosPerm, context->HelperResolveSystemIdtf("_concept_timer"), new_tup);
    }
}


void dfs(const std::unique_ptr<ScMemoryContext>& context, ScAddr current_vertex, ScAddr parent_vertex, int time)
{
    // Mark current_vertex as visited
    context->CreateEdge(ScType::EdgeAccessVarPosPerm, context->HelperResolveSystemIdtf("_visited_vertexes"), current_vertex);

    set_tin(context, current_vertex, time);

    set_tup(context, current_vertex, time++);

    ScIterator5Ptr tin_finder = context->Iterator5(
            current_vertex,
            ScType::EdgeDCommonVar,
            ScType::NodeVar,
            ScType::EdgeAccessVarPosPerm,
            context->HelperResolveSystemIdtf("_tin")
            );

    ScAddr tin(0);
    if (tin_finder->Next()) tin = tin_finder->Get(2);

    ScIterator5Ptr tup_finder = context->Iterator5(
            current_vertex,
            ScType::EdgeDCommonVar,
            ScType::NodeVar,
            ScType::EdgeAccessVarPosPerm,
            context->HelperResolveSystemIdtf("_tup")
            );

    ScAddr tup(0);
    if (tup_finder->Next()) tup = tup_finder->Get(2);

    int children = 0;

    // Iterator among children vertexes of current vertex
    ScIterator3Ptr to_vertex_iter = context->Iterator3(current_vertex, ScType::EdgeUCommonConst, ScType::Unknown);
    while (to_vertex_iter->Next())
    {
        // to_vertex is next vertex to start dfs from
        ScAddr to_vertex = to_vertex_iter->Get(2);
        if (to_vertex == parent_vertex) continue;
        ScIterator3Ptr if_checked = context->Iterator3(context->HelperResolveSystemIdtf("_visited_vertexes"), ScType::EdgeAccessVarPosPerm, to_vertex);
        if (if_checked->Next())
        {
            ScIterator5Ptr tin_to_vertex = context->Iterator5(
                    to_vertex,
                    ScType::EdgeDCommonVar,
                    ScType::NodeVar,
                    ScType::EdgeAccessVarPosPerm,
                    context->HelperResolveSystemIdtf("_tin")
                    );
            ScAddr tin_to(ScType::Unknown);
            if (tin_to_vertex->Next()) tin_to = tin_to_vertex->Get(2);
            set_tup(context, tup, min(context, tin_to, tup));
        }
        else
        {
            dfs(context, to_vertex, current_vertex, time);

            ScIterator5Ptr tup_to_vertex = context->Iterator5(
                    to_vertex,
                    ScType::EdgeDCommonVar,
                    ScType::NodeVar,
                    ScType::EdgeAccessVarPosPerm,
                    context->HelperResolveSystemIdtf("_tup")
                    );
            ScAddr tup_to(ScType::Unknown);
            if (tup_to_vertex->Next()) tup_to = tup_to_vertex->Get(2);
            set_tup(context, tup, min(context, tup_to, tup));


            int tup_to_value = std::stoi(context->HelperGetSystemIdtf(tup_to));
            int tin_value = std::stoi(context->HelperGetSystemIdtf(tin));

            if ((tup_to_value >= tin_value) && (current_vertex != parent_vertex))
            {
                ScIterator3Ptr check_duble = context->Iterator3(
                        context->HelperResolveSystemIdtf("articulation_point"),
                        ScType::EdgeAccessConstPosPerm,
                        current_vertex
                        );
                if (!check_duble->Next())
                    context->CreateEdge(ScType::EdgeAccessConstPosPerm, context->HelperResolveSystemIdtf("articulation_point"), current_vertex);
            }
            children++;
        }
    }

    if ((current_vertex == parent_vertex) && children > 1) 
    {
        ScIterator3Ptr check_duble = context->Iterator3(
                context->HelperResolveSystemIdtf("articulation_point"),
                ScType::EdgeAccessConstPosPerm,
                current_vertex
                );
        if (!check_duble->Next())
            context->CreateEdge(ScType::EdgeAccessConstPosPerm, context->HelperResolveSystemIdtf("articulation_point"), current_vertex);
    }
}
