/*
    This file demonstrates how to work with sets.
*/

#include <MVault.h>

void Join(mvlt::Vault& vlt)
{
    mvlt::VaultRecordSet vrs1, vrs2;

    vlt.RequestLess("A", 3, vrs1);
    vlt.RequestGreater("B", 7, vrs2);

    // Append records from vrs2 to vrs1
    vrs1.Join(vrs2);
    
    // Print result
    std::cout << "Join:" << std::endl;
    vrs1.Print();
}

void Exclude(mvlt::Vault& vlt)
{
    mvlt::VaultRecordSet vrs1, vrs2;

    vlt.RequestLess("A", 7, vrs1);
    vlt.RequestGreater("B", 3, vrs2);

    // Remove records from vrs1 that are in vrs2
    vrs1.Exclude(vrs2);
    
    // Print result
    std::cout << "\n" << "Exclude:" << std::endl;
    vrs1.Print();
}

void Intersect(mvlt::Vault& vlt)
{
    mvlt::VaultRecordSet vrs1, vrs2;

    vlt.RequestLess("A", 7, vrs1);
    vlt.RequestGreater("B", 3, vrs2);

    // Remove records from vrs1 that are not in vrs2
    vrs1.Intersect(vrs2);
    
    // Print result
    std::cout << "\n" << "Intersect:" << std::endl;
    vrs1.Print();
}

void Union(mvlt::Vault& vlt)
{
    mvlt::VaultRecordSet vrs1, vrs2, vrs3;

    vlt.RequestLess("A", 3, vrs1);
    vlt.RequestGreater("B", 7, vrs2);

    // Union records from vrs1 and vrs2
    Union(vrs1, vrs2, vrs3);
    
    // Print result
    std::cout << "\n" << "Union:" << std::endl;
    vrs3.Print();
}

void Intersection(mvlt::Vault& vlt)
{
    mvlt::VaultRecordSet vrs1, vrs2, vrs3;

    vlt.RequestLess("A", 7, vrs1);
    vlt.RequestGreater("B", 3, vrs2);

    // Remove records from vrs1 that are not in vrs1
    Intersection(vrs1, vrs2, vrs3);
    
    // Print result
    std::cout << "\n" << "Intersection:" << std::endl;
    vrs3.Print();
}

int main()
{
    // Create and fill vault
    mvlt::Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);

    // Fill vault with data
    for (int i = 0; i < 10; ++i)
        vlt.CreateRecord({{"A", i}, {"B", i}});

    Join(vlt);
    Exclude(vlt);
    Intersect(vlt);

    Union(vlt);
    Intersection(vlt);
}