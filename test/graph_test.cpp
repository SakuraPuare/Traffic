#include "graph.hpp"
#include <gtest/gtest.h>

TEST(GraphTest, AddNode) {
    Graph g;
    g.AddNode("A");
    g.AddNode("B");
    g.AddNode("C");

    EXPECT_EQ(g.node.size(), 3);
    EXPECT_EQ(g.node[0].name, "A");
    EXPECT_EQ(g.node[1].name, "B");
    EXPECT_EQ(g.node[2].name, "C");
}

TEST(GraphTest, AddDuplicateNode) {
    Graph g;
    g.AddNode("A");
    g.AddNode("B");
    g.AddNode("C");
    g.AddNode("A");

    EXPECT_EQ(g.node.size(), 3);
}

TEST(GraphTest, DeleteNode) {
    Graph g;
    g.AddNode("A");
    g.AddNode("B");
    g.AddNode("C");
    g.DeleteNode("B");

    EXPECT_EQ(g.node.size(), 2);
    EXPECT_EQ(g.node[0].name, "A");
    EXPECT_EQ(g.node[1].name, "C");
}

TEST(GraphTest, SetNode) {
    Graph g;
    g.AddNode("A");
    g.AddNode("B");
    g.AddNode("C");
    g.SetNode("B", "D");

    EXPECT_EQ(g.node.size(), 3);
    EXPECT_EQ(g.node[1].name, "D");
}

TEST(GraphTest, AddEdge) {
    Graph g;
    g.AddNode("A");
    g.AddNode("B");
    g.AddNode("C");
    g.AddEdge("A", "B", 10);
    g.AddEdge("B", "C", 20);

    EXPECT_EQ(g.edge.size(), 2);
    EXPECT_EQ(g.matrix[0][1], 10);
    EXPECT_EQ(g.matrix[1][2], 20);
}

TEST(GraphTest, AddBiEdge) {
    Graph g;
    g.AddNode("A");
    g.AddNode("B");
    g.AddNode("C");
    g.AddBiEdge("A", "B", 10);

    EXPECT_EQ(g.edge.size(), 2);
    EXPECT_EQ(g.matrix[0][1], 10);
    EXPECT_EQ(g.matrix[1][0], 10);
}

TEST(GraphTest, DeleteEdge) {
    Graph g;
    g.AddNode("A");
    g.AddNode("B");
    g.AddNode("C");
    g.AddEdge("A", "B", 10);
    g.AddEdge("B", "C", 20);
    g.DeleteEdge("A", "B");

    EXPECT_EQ(g.edge[0], *g.GetEdge("B", "C"));
    EXPECT_EQ(g.edge.size(), 1);
    EXPECT_EQ(g.matrix[0][1], 0);
}

TEST(GraphTest, GetMatrix) {
    Graph g;
    g.AddNode("A");
    g.AddNode("B");
    g.AddNode("C");
    g.AddEdge("A", "B", 10);
    g.AddEdge("B", "C", 20);

    EXPECT_EQ(g.GetMatrix("A", "B"), 10);
    EXPECT_EQ(g.GetMatrix("B", "C"), 20);
    EXPECT_EQ(g.GetMatrix("A", "C"), 0);
}

TEST(GraphTest, GetNode) {
    Graph g;
    g.AddNode("A");
    g.AddNode("B");
    g.AddNode("C");

    EXPECT_EQ(g.GetNode("B")->name, "B");
    EXPECT_EQ(g.GetNode(2)->name, "C");
    EXPECT_EQ(g.GetNode("D"), nullptr);
}

TEST(GraphTest, GetEdge) {
    Graph g;
    g.AddNode("A");
    g.AddNode("B");
    g.AddNode("C");
    g.AddEdge("A", "B", 10);
    g.AddEdge("B", "C", 20);

    EXPECT_EQ(g.GetEdge("A", "B")->distance, 10);
    EXPECT_EQ(g.GetEdge("B", "C")->distance, 20);
    EXPECT_EQ(g.GetEdge("A", "C"), nullptr);
}


TEST(GraphTest, GetAllPath) {
    Graph g;
    g.AddNode("A");
    g.AddNode("B");
    g.AddNode("C");
    g.AddNode("D");
    g.AddEdge("A", "B", 1);
    g.AddEdge("A", "C", 2);
    g.AddEdge("B", "D", 3);
    g.AddEdge("C", "D", 4);

    auto paths = g.GetAllPath("A", "D");
    ASSERT_EQ(paths.size(), 2);

    // Check the first path
    ASSERT_EQ(paths[0].size(), 2);
    ASSERT_EQ(paths[0][0].from->name, "A");
    ASSERT_EQ(paths[0][0].to->name, "B");
    ASSERT_EQ(paths[0][1].from->name, "B");
    ASSERT_EQ(paths[0][1].to->name, "D");

    // Check the second path
    ASSERT_EQ(paths[1].size(), 2);
    ASSERT_EQ(paths[1][0].from->name, "A");
    ASSERT_EQ(paths[1][0].to->name, "C");
    ASSERT_EQ(paths[1][1].from->name, "C");
    ASSERT_EQ(paths[1][1].to->name, "D");
}