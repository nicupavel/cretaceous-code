#include <stdio.h>
#include <asoundlib.h>

#define VOLUME 50

int main (void) 
{
    int err;
    snd_hctl_t *hctl;
    snd_ctl_elem_id_t *id;
    snd_ctl_elem_value_t *control;
    
    err = snd_hctl_open(&hctl, "hw:1", 0));
    err = snd_hctl_load(hctl);
    
    snd_ctl_elem_id_alloca(&id);
    snd_ctl_elem_value_alloca(&control);
    
    snd_ctl_elem_id_set_interface(id, SND_CTL_ELEM_IFACE_MIXER);
    snd_ctl_elem_id_set_name(id, "Master");
    snd_hctl_elem_t *elem = snd_hctl_find_elem(hctl, id);
    
    snd_ctl_elem_value_set_id(control, id);    
    snd_ctl_elem_value_set_integer(control, 0, VOLUME);
    
    err = snd_hctl_elem_write(elem, control);
    snd_hctl_close(hctl);

    return 0;
}