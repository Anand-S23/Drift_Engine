#include "drift_ui.h"

internal ui_id UIIDNull()
{
    ui_id id = { -1, -1 };
    return id;
}

internal ui_id UIIDInit(u32 primary, u32 secondary)
{
    ui_id id = { primary, secondary };
    return id;
}

internal b32 UIIDEqual(ui_id id1, ui_id id2)
{
    return (id1.primary == id2.primary &&
            id1.secondary == id2.secondary);
}

internal v4 UIGetPannelAutoLayoutRect(ui *ui, v4 something)
{
    v4 rect = {0};
}


internal void UIBeginFrame(ui *ui, renderer *renderer)
{
    ui->mouse_x = platform->mouse_x;
    ui->mouse_y = platform->mouse_y;
    ui->left_mouse_down = platform->left_mouse_down;
    ui->right_mouse_down = platform->right_mouse_down;

    ui->renderer = renderer;
    ui->widget_count = 0;
}

internal void UIEndFrame(ui *ui)
{
    for(u32 i = 0; i < ui->widget_count; ++i)
    {
        ui_widget *widget = ui->widgets + i;
        
        widget->t_hot +=
            ((f32)(!!UIIDEqual(ui->hot, widget->id)) - widget->t_hot);

        widget->t_active +=
            ((f32)(!!UIIDEqual(ui->active, widget->id)) - widget->t_active);

        switch(widget->type)
        {
            case UI_WIDGET_button:
            {
                v4 color = {
                    0.6f + widget->t_hot * 0.4f - widget->t_active * 0.5f,
                    0.6f + widget->t_hot * 0.4f - widget->t_active * 0.5f,
                    0.6f + widget->t_hot * 0.4f - widget->t_active * 0.5f,
                    0.6f + widget->t_hot * 0.4f - widget->t_active * 0.5f,
                };
                
                RenderRect(ui->renderer, widget->position, widget->size, color);
            } break;
            
            case UI_WIDGET_slider:
            {
                RenderRect(ui->renderer, widget->position, widget->size,
                           v4(0.6f, 0.6f, 0.6f, 1.f));

                RenderRect(ui->renderer, widget->position, 
                           v2(widget->size.width * widget->slider.value,
                              widget->size.height),
                           v4(0.8f, 0.8f, 0.8f, 1.f));
            } break;
            
            default:
            {
                DriftLogWarning("Invalid widget type");
            } break;
        }
    }
}

internal void UIMenu(ui *ui, v2 start_pos, v2 element_size)
{
}

internal b32 UIButtonInternal(ui *ui, ui_id id, char *text,
                              v2 position, v2 size)
{
    Assert(ui->widget_count < UI_MAX_WIDGETS);
    
    b32 is_triggered = 0;
    
    b32 cursor_is_over = (ui->mouse_x >= position.x &&
                          ui->mouse_x <= position.x + size.width &&
                          ui->mouse_y >= position.y &&
                          ui->mouse_y <= position.y + size.height);
    
    if (!UIIDEqual(ui->hot, id) && cursor_is_over)
    {
        ui->hot = id;
    }
    else if (UIIDEqual(ui->hot, id) && !cursor_is_over)
    {
        ui->hot = UIIDNull();
    }
    
    if (UIIDEqual(ui->active, id))
    {
        if(!ui->left_mouse_down)
        {
            is_triggered = UIIDEqual(ui->hot, id);
            ui->active = UIIDNull();
            ui->open_menu = UIIDNull();
        }
    }
    else
    {
        if (UIIDEqual(ui->hot, id))
        {
            if (ui->left_mouse_down)
            {
                ui->active = id;
            }
        }
    }
    
    ui_widget *widget = ui->widgets + ui->widget_count++;
    widget->type = UI_WIDGET_button;
    widget->id = id;
    widget->position = position;
    widget->size = size;
    
    return is_triggered;
}

internal b32 UIButton(ui *ui, ui_id id, char *text,
                      v2 position, v2 size)
{
    return UIButtonInternal(ui, id, text, position, size);
}

internal f32 UISliderInternal(ui *ui, ui_id id, v2 position,
                              v2 size, f32 value)
{
    Assert(ui->widget_count < UI_MAX_WIDGETS);
   
    b32 cursor_is_over = (ui->mouse_x >= position.x &&
                          ui->mouse_x <= position.x + size.width &&
                          ui->mouse_y >= position.y &&
                          ui->mouse_y <= position.y + size.height);
    
    if (!UIIDEqual(ui->hot, id) && cursor_is_over)
    {
        ui->hot = id;
    }
    else if (UIIDEqual(ui->hot, id) && !cursor_is_over)
    {
        ui->hot = UIIDNull();
    }
    
    if (!UIIDEqual(ui->active, id))
    {
        if (UIIDEqual(ui->hot, id))
        {
            if (ui->left_mouse_down)
            {
                ui->active = id;
            }
        }
    }
    
    if (UIIDEqual(ui->active, id))
    {
        if (ui->left_mouse_down)
        {
            value = (ui->mouse_x - position.x) / size.width;
        }
        else
        {
            ui->active = UIIDNull();
            ui->open_menu = UIIDNull();
        }
    }
    
    value = Max(Min(value, 1.f), 0.f);
    
    ui_widget *widget = ui->widgets + ui->widget_count++;
    widget->type = UI_WIDGET_slider;
    widget->id = id;
    widget->position = position;
    widget->size = size;
    widget->slider.value = value;
    
    return value;
}

internal f32 UISlider(ui *ui, ui_id id, v2 position, v2 size, f32 value)
{
    return UISliderInternal(ui, id, position, size, value);
}

