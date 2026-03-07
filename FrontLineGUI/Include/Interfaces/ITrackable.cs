using System;

// RPECK 02/03/2026 - Added to provide the means to keep track of different models in EntityFramework
// Ensures we are able keep the likes of CreateAt / UpdatedAt working as needed
namespace FrontLineGUI.Include.Interfaces
{
    interface ITrackable
    {

        DateTime CreatedAt { get; set; }
        DateTime? UpdatedAt { get; set; }
    }
}