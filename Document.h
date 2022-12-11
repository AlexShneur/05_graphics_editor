#pragma once
#include "Primitive.h"
#include <memory>
#include <vector>
#include <algorithm>

enum class DocumentState
{
    created, imported, exported, primitiveCreated, primitiveRemoved
};

class Document
{
public:

    Document(const std::string& _name, const DocumentState& _state)
        : name(_name), docState(_state)
    {
        exported_name = "";
    }

    const std::string& get_name() const
    {
        return name;
    }

    const DocumentState& get_state() const
    {
        return docState;
    }

    const std::string& get_exported_name() const
    {
        return exported_name;
    }

    auto get_active_primitive() const
    {
        return *activePrimitiveIt;
    }

    const void export_to_file(const std::string& fileName)
    {
        exported_name = fileName;
        docState = DocumentState::exported;
    }

    void create_primitive(std::shared_ptr<IDrawPrimitive> primitive)
    {
        //primitive->draw();
        primitives.emplace_back(primitive);
        activePrimitiveIt = end(primitives) - 1;
        docState = DocumentState::primitiveCreated;
    }

    bool remove_primitive(std::shared_ptr<IDrawPrimitive> primitive)
    {
        auto it = std::remove(begin(primitives), end(primitives), primitive);
        if (it != end(primitives))
        {
            primitives.erase(it);
            docState = DocumentState::primitiveRemoved;
            return true;
        }
        return false;
    }

private:
    std::string name;
    DocumentState docState;
    std::string exported_name;
    std::vector<std::shared_ptr<IDrawPrimitive>> primitives;
    std::vector<std::shared_ptr<IDrawPrimitive>>::iterator activePrimitiveIt;
};

bool operator==(const Document& doc, const std::string& name)
{
    return doc.get_name() == name;
}