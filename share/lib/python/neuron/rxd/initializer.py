import itertools

# TODO: monkey-patch everything that requires an init so only even attempts once

def _do_ion_register():
    from . import species
    for obj in species._all_species:
        obj = obj()
        if obj is not None:
            obj._ion_register()


has_initialized = False
def _do_init():
    global has_initialized
    from . import rxd
    if has_initialized and rxd.last_structure_change_cnt == rxd._structure_change_count.value: return
    from . import species, region
    if len(species._all_species) > 0:
        local_has_init  = has_initialized
        has_initialized = True
        rxd.last_structure_change_cnt = rxd._structure_change_count.value
        # TODO: clean this up so not repetitive; can't do it super cleanly because of the multiple phases of species
        for obj in region._all_regions:
            obj = obj()
            if obj is not None:
                obj._do_init()
        if not local_has_init:
            # NOTE: for species, must do all _do_init2s (1D alloc) before _do_init3s (3D alloc) to keep nodes arranged nicely
            for obj in species._all_species:
                obj = obj()
                if obj is not None:
                    obj._do_init1()
        for obj in species._all_species:
            obj = obj()
            if obj is not None:
                obj._do_init2()
        for obj in species._all_species:
            obj = obj()
            if obj is not None:
                obj._do_init3()
        if not local_has_init:
            #TODO: allow extracellular grid to change
            for obj in species._all_species:
                obj = obj()
                if obj is not None:
                    obj._do_init4()
        for obj in species._all_species:
            obj = obj()
            if obj is not None:
                obj._do_init5()
        for obj in rxd._all_reactions:
            obj = obj()
            if obj is not None:
                obj._do_init()
        rxd._init()
        




def is_initialized():
    return has_initialized

def assert_initialized(msg=''):
    from . import species
    if not has_initialized and len(species._all_species) > 0:
        if len(msg):
            msg = ': ' + msg
        from .rxdException import RxDException
        raise RxDException('invalid operation; rxd module not initialized' + msg)
