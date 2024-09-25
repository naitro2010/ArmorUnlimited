#pragma warning( disable : 4100 4189 )
#include "SKSE/API.h"
#include "SKSE/Interfaces.h"
#include "RE/N/NiSmartPointer.h"
#include "REL/Relocation.h"
#include <windows.h>
#undef GetObject
#define DLLEXPORT __declspec(dllexport)
//#define FOR640
#define FOR_VR_1_4_150
//#define FOR1170
#ifdef FOR640
static uint64_t slotpatch_offset = 0x646d4e;
#endif
#ifdef FOR1170
static uint64_t slotpatch_offset = 0x6a0b3e;
#endif
#ifdef FOR_VR_1_4_150
static uint64_t slotpatch_offset = 0x61568e;
#endif
#ifdef FOR640
static uint64_t biped_1p_offset = 0x8f0;
static uint64_t biped_3p_offset = 0x268;
static uint64_t addwornitemvtable0 = 0x174cdc0;
static uint64_t addwornitemvtable1 = 0x1754ae8;
static uint64_t addwornitemvtable2 = 0x175af28;
static uint64_t addwornitemhooks[] = { 0x645e2d, 0x645ead, 0x645f2b, 0x646008 };
static uint64_t ret_from_bipedanim_dtor_offset = 0x1d2fd4;
static uint64_t call_unequipall_from_dtor_offset = 0x1d2f92;
static uint64_t unequip_all_offset = 0x1d2480;
static uint64_t init_worn_armor = 0x239850;
static uint64_t prepare_equip_biped_hook0 = 0x37e8ce;
static uint64_t prepare_equip_biped_hook1 = 0x37d808;
static uint64_t prepare_equip_biped_hook2 = 0x37d762;
static uint64_t prepare_equip_biped_hook3 = 0x1d3cd9;
static uint64_t clear_3d_hooks[] = { 0x62d2dc, 0x62d2f7, 0x6cd14, 0x6cd1b8, 0x6cd206, 0x8f3d18, 0x8f3d87, 0x9ae34f };
static uint64_t unequip_hook = 0x6703c9;
static uint64_t real_unequip = 0x672a10;
static uint64_t equip_biped = 0x1d40a0;
static uint64_t construct_biped_offset = 0x1d2e60;
static uint64_t biped_dtor = 0x1d2f80;
#endif
#ifdef FOR1170
static uint64_t biped_1p_offset = 0x8f0;
static uint64_t biped_3p_offset = 0x268;
static uint64_t addwornitemvtable0 = 0x189db28;
static uint64_t addwornitemvtable1 = 0x18a5810;
static uint64_t addwornitemvtable2 = 0x18abc78;
static uint64_t addwornitemhooks[] = { 0x69fc1d, 0x69fc9d, 0x69fd1b, 0x69fdf8 };
static uint64_t ret_from_bipedanim_dtor_offset = 0x2124b4;
static uint64_t call_unequipall_from_dtor_offset = 0x212472;
static uint64_t unequip_all_offset = 0x212560;
static uint64_t init_worn_armor = 0x279320;
static uint64_t prepare_equip_biped_hook0 = 0x3be42e;
static uint64_t prepare_equip_biped_hook1 = 0x3bd368;
static uint64_t prepare_equip_biped_hook2 = 0x3bd2c2;
static uint64_t prepare_equip_biped_hook3 = 0x2131b9;
static uint64_t clear_3d_hooks[] = { 0x726f7e, 0x726fe8, 0x727036, 0xa0c76f, 0x6870cc, 0x6870e7, 0x2136aa };
static uint64_t unequip_hook = 0x6ca1c9;
static uint64_t real_unequip = 0x6cc810;
static uint64_t equip_biped = 0x213580;
static uint64_t construct_biped_offset = 0x212340;
static uint64_t biped_dtor = 0x212460;
#endif
#ifdef FOR_VR_1_4_150
static uint64_t biped_1p_offset = 0xfe8;
static uint64_t biped_3p_offset = 0x260;
static uint64_t addwornitemvtable0 = 0x16ceb40;
static uint64_t addwornitemvtable1 = 0x16d7098;
static uint64_t addwornitemvtable2 = 0x16e24e8;
static uint64_t addwornitemhooks[] = { 0x61487b, 0x6148fc, 0x614979, 0x614a58, 0x614a8c };
static uint64_t jmp_from_bipedanim_dtor_offset_vr = 0x1d624e;
static uint64_t call_unequipall_from_dtor_offset = 0x1d6212;
static uint64_t unequip_all_offset = 0x1d6530;
static uint64_t init_worn_armor = 0x239e00;
static uint64_t prepare_equip_biped_hook0 = 0x1d7003;
static uint64_t prepare_equip_biped_hook1 = 0x373fb2;
static uint64_t prepare_equip_biped_hook2 = 0x374058;
static uint64_t prepare_equip_biped_hook3 = 0x374e4e;
static uint64_t clear_3d_hooks[] = { 0x1d7024, 0x1d73c1, 0x1de722, 0x5fcbcd, 0x5fcbe8, 0x69c77e, 0x69c7e6,0x69c834};
static uint64_t unequip_hook = 0x6412d8;
static uint64_t real_unequip = 0x6436c0;
static uint64_t equip_biped = 0x1d7450;
static uint64_t construct_biped_offset = 0x1d60f0;
static uint64_t biped_dtor = 0x1d6200;
static uint64_t eh_vector_destructor_iterator_offset = 0x138aa7c;
#endif
void InitializeLog([[maybe_unused]] spdlog::level::level_enum a_level = spdlog::level::info)
{
#ifndef NDEBUG
	auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
	auto path = logger::log_directory();
	if (!path) {
		util::report_and_fail("Failed to find standard logging directory"sv);
	}

	*path /= std::format("{}.log"sv, Plugin::NAME);
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

	const auto level = a_level;

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));
	log->set_level(level);
	log->flush_on(spdlog::level::info);

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%t] [%s:%#] %v");
}

std::map<RE::BipedAnim*, std::map<int,RE::BipedAnim*>> BipedAnimToExtraWorn;
std::recursive_mutex g_bipedstate_mutex;
void (*orig_equiparmorstuff)(uint64_t arg1,uint64_t arg2,uint64_t arg3,
                                uint64_t arg4, uint64_t arg5) = nullptr;
uint64_t (*orig_addwornitem_fn)(RE::Actor *actor, RE::TESBoundObject *item, int32_t count, uint64_t forceEquip, uint64_t arg4,
                            uint64_t arg5)=nullptr;
void (*real_unequip_fn)(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5) = nullptr;
    void (*orig_unequip_fn)(RE::BipedAnim *, RE::BIPOBJECT*, char, uint64_t) = nullptr;
RE::BipedAnim *(*BipedAnimconstruct)(RE::BipedAnim *, void *, void *) = nullptr;
void (*orig_equipbiped_fn)(RE::BipedAnim *, uint64_t,uint64_t,uint64_t,uint64_t) = nullptr;
void (*orig_preparebiped_fn)(RE::TESObjectARMO * armor, RE::TESRace* race,
                             RE::BSTSmartPointer<RE::BipedAnim> *bipedanim_sptr, uint64_t param_4) = nullptr;
static bool unequip_mode = true;
void (*orig_unequip_all_fn)(RE::BipedAnim *, uint64_t, uint64_t) = nullptr;
static RE::TESObjectARMO* current_prepared_armor=nullptr;
static RE::BipedAnim *to_destroy_bipedanim = nullptr;
void UnequipAllBipedDtor(RE::BipedAnim *bipedanim, uint64_t arg2, uint64_t arg3) {
    std::lock_guard<std::recursive_mutex> lock(g_bipedstate_mutex);
    orig_unequip_all_fn(bipedanim, arg2, arg3);
    if (to_destroy_bipedanim == nullptr) {
        to_destroy_bipedanim = bipedanim;
    }
}

void HookAfterBipedDtor(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
    std::lock_guard<std::recursive_mutex> lock(g_bipedstate_mutex);
#ifdef FOR_VR_1_4_150
	auto eh_vector_destructor_iterator = (void (*)(uint64_t, uint64_t, uint64_t, uint64_t))(REL::Offset(eh_vector_destructor_iterator_offset).address());
	eh_vector_destructor_iterator(arg1, arg2, arg3, arg4);
#endif
    if (to_destroy_bipedanim != nullptr) {
        if (BipedAnimToExtraWorn.contains(to_destroy_bipedanim)) {
            
            
            for (auto &p : BipedAnimToExtraWorn[to_destroy_bipedanim]) {
                auto biped_dtor_3d = (void (*)(RE::BipedAnim *, uint64_t, uint64_t))(REL::Offset(biped_dtor).address());
				auto biped_clear_3d =
					(void (*)(RE::BipedAnim*, uint64_t, uint64_t))(REL::Offset(unequip_all_offset).address());
                if (p.second != nullptr) {
                    biped_dtor_3d(p.second, 0, 0);
                    free((void *)p.second);
                }
                p.second = nullptr;
                
                
            }
        }
        BipedAnimToExtraWorn.erase(to_destroy_bipedanim);
        to_destroy_bipedanim = nullptr;
    }
}
void Clear3DHook(RE::BipedAnim * bipedanim, uint64_t arg2, uint64_t arg3) {
    std::lock_guard<std::recursive_mutex> lock(g_bipedstate_mutex);
    auto biped_clear_3d =
		(void (*)(RE::BipedAnim*, uint64_t, uint64_t))(REL::Offset(unequip_all_offset).address());
    biped_clear_3d(bipedanim,arg2,arg3);
    if (BipedAnimToExtraWorn.contains(bipedanim)) {
        for (auto p : BipedAnimToExtraWorn[bipedanim]) {
            biped_clear_3d(p.second, 0, 0);
        }
    }
}
void PrepareEquipBiped(RE::TESObjectARMO *armor, RE::TESRace *race, RE::BSTSmartPointer<RE::BipedAnim> *bipedanim_sptr,
                       uint64_t param_4) {
    std::lock_guard<std::recursive_mutex> lock(g_bipedstate_mutex);
    

    if (bipedanim_sptr != nullptr && bipedanim_sptr->get() != nullptr) {
        if (bipedanim_sptr->get()->actorRef.get().get()->As<RE::Actor>()->GetSkin() == armor) {
            return orig_preparebiped_fn(armor, race, bipedanim_sptr, param_4);
        }
        auto biped_equip_finish=(void (*)(RE::BipedAnim *, uint64_t, uint64_t, uint64_t,uint64_t))(REL::Offset(equip_biped).address());
        auto biped_clear_3d =
			(void (*)(RE::BipedAnim*, uint64_t, uint64_t))(REL::Offset(unequip_all_offset).address());
        RE::BipedAnim * bipedanim=bipedanim_sptr->get();
        if ((bipedanim->actorRef.get()).get() && bipedanim->root != nullptr &&
                bipedanim->actorRef.get().get()->formType == RE::FormType::ActorCharacter
            ) {
            if (!BipedAnimToExtraWorn.contains(bipedanim)) {
                BipedAnimToExtraWorn.insert_or_assign(bipedanim, std::map<int, RE::BipedAnim *>());
            }
            if (armor != nullptr) {
                if (armor->formType == RE::FormType::Armor) {
                    bool found = false;
                    if (BipedAnimToExtraWorn[bipedanim->actorRef.get().get()->GetBiped1(false).get()].contains(
                            armor->formID)) {
                        found = true;
                    }
                    if (found == false) {
                        RE::BipedAnim *(*construct_biped)(RE::BipedAnim *, RE::Actor *, RE::NiNode *) =
                            (RE::BipedAnim *
                             (*)(RE::BipedAnim *, RE::Actor *, RE::NiNode *))(REL::Offset(construct_biped_offset).address());
                        RE::BipedAnim *new_biped = (RE::BipedAnim *)malloc(0x2778);
                        construct_biped(new_biped, bipedanim->actorRef.get().get()->As<RE::Actor>(), bipedanim->root);
                        new_biped->IncRef();
                        BipedAnimToExtraWorn[bipedanim].insert(
                            std::pair(armor->formID & 0xFFFFFFFF, (RE::BipedAnim *)new_biped));

                        
                    }
                    if (BipedAnimToExtraWorn[bipedanim][armor->formID&0xFFFFFFFF] != nullptr) {
                        std::pair p(armor->formID & 0xFFFFFFFF,
                                    BipedAnimToExtraWorn[bipedanim][armor->formID & 0xFFFFFFFF]);
                            biped_clear_3d(p.second, 0, 0);
                            bool done2 = false;
                            bipedanim->IncRef();
                            uint64_t *actor_raw = (uint64_t *)bipedanim->actorRef.get().get();
							if ((actor_raw[biped_1p_offset / 8] != actor_raw[biped_3p_offset/8]) &&
                                actor_raw[biped_1p_offset / 8] != 0x0 && actor_raw[biped_1p_offset / 8] == (uint64_t)bipedanim) {
                                  
                                RE::BSTSmartPointer new_biped(p.second);
                                orig_preparebiped_fn(armor, race, &new_biped,param_4);
                                
                                    for (uint64_t i = 0; i < 0x2a; i++) {
                                        if (RE::TESObjectARMA *aa = armor->GetArmorAddonByMask(
                                                bipedanim->actorRef.get().get()->As<RE::Actor>()->GetRace(),
                                                (RE::BIPED_MODEL::BipedObjectSlot)(1ULL << i))) {

                                        
                                            p.second->objects[i].item = armor;
                                            p.second->objects[i].addon = aa;
                                            p.second->objects[i].part = &aa->bipedModel1stPersons[param_4];
                                            p.second->objects[i].skinTexture = aa->skinTextures[param_4];
                                            if (p.second->objects[i].partClone) {
                                                if (p.second->root) {
                                                    //p.second->root->AttachChild(p.second->objects[i].partClone.get(),
                                                                                //true);
                                                }
                                            }
                                        }
                                    }
                                
                                done2 = true;
                            } else if (actor_raw[biped_3p_offset / 8] != 0x0 &&
                                actor_raw[biped_3p_offset / 8] == (uint64_t)bipedanim) {
                                
                                RE::BSTSmartPointer new_biped(p.second);
                                orig_preparebiped_fn(armor, race, &new_biped, param_4);
                                
                                    for (int i = 0; i < 0x2a; i++) {
                                        if (RE::TESObjectARMA *aa = armor->GetArmorAddonByMask(
                                                bipedanim->actorRef.get().get()->As<RE::Actor>()->GetRace(),
                                                (RE::BIPED_MODEL::BipedObjectSlot)(1ULL << i))) {
                                        p.second->objects[i].item = armor;
                                        p.second->objects[i].addon = aa;
                                        p.second->objects[i].part = &aa->bipedModels[param_4];
                                        p.second->objects[i].skinTexture = aa->skinTextures[param_4];
                                        
                                        if (p.second->objects[i].partClone) {
                                            if (p.second->root) {
                                                //p.second->root->AttachChild(p.second->objects[i].partClone.get(), true);
                                            }
                                        }
                                    }
                                
                                    
                                }
                                done2 = true;
                                //actor_raw[biped_3p_offset / 8] = (uint64_t)bipedanim;
                            }
                            biped_equip_finish(p.second, 0, 1, 0, 0);
                            bipedanim->DecRef();
                            if (done2 == true) {
                                return;
                            }

                        }
                    }
                }
            }
        }
    
    
    orig_preparebiped_fn(armor, race, bipedanim_sptr, param_4);
}
void UnequipHook(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5) { 
    std::lock_guard<std::recursive_mutex> lock(g_bipedstate_mutex);
    RE::TESBoundObject *item = (RE::TESBoundObject*)arg3;
    bool done = false;
    real_unequip_fn(arg1, arg2, arg3, arg4, arg5);
    //auto biped_dtor_3d = (void (*)(RE::BipedAnim *, uint64_t, uint64_t))(REL::Offset(0x212460).address());
	auto biped_clear_3d = (void (*)(RE::BipedAnim*, uint64_t, uint64_t))(REL::Offset(unequip_all_offset).address());
    if (item && item->IsArmor()) {
        if (RE::TESObjectARMO* armor = item->As<RE::TESObjectARMO>()) {
            if (RE::Actor* actor = (RE::Actor*)arg2) {
                if (actor->IsDisabled()) {
                    return;
                }
                if (actor->formType == RE::FormType::ActorCharacter) {
                    if (actor->GetSkin()==armor) {
                        return;
                    }
                    bool no1P = false;
                    if (actor->GetBiped1(false) == actor->GetBiped1(true)) {
                        no1P = true;
                    }
                    if (actor->GetBiped1(false)) {
                        RE::BipedAnim *bipedanim = actor->GetBiped1(false).get();
                        if (!BipedAnimToExtraWorn.contains(actor->GetBiped1(false).get())) {
                            BipedAnimToExtraWorn.insert_or_assign(bipedanim, std::map<int, RE::BipedAnim *>());
                        }
                        bool found = false;
                        if (BipedAnimToExtraWorn[actor->GetBiped1(false).get()].contains(item->formID&0xFFFFFFFF)) {
                            auto p = std::pair(item->formID&0xFFFFFFFF,BipedAnimToExtraWorn[actor->GetBiped1(false).get()][item->formID&0xFFFFFFFF]);
                        
                            if ((p.first & 0xFFFFFFFF) == (item->formID & 0xFFFFFFFF)) {
                                biped_clear_3d(p.second, 0, 0);
                                
                                done = true;
                                found = true;

                            }
                        }
                    }
                    if (no1P == false) {
                        if (actor->GetBiped1(true)) {
                            RE::BipedAnim *bipedanim = actor->GetBiped1(true).get();
                            if (!BipedAnimToExtraWorn.contains(actor->GetBiped1(true).get())) {
                                BipedAnimToExtraWorn.insert_or_assign(bipedanim, std::map<int, RE::BipedAnim *>());
                            }
                            bool found = false;
                            if (BipedAnimToExtraWorn[actor->GetBiped1(true).get()].contains(item->formID&0xFFFFFFFF)) {
                                auto p = std::pair(item->formID&0xFFFFFFFF,BipedAnimToExtraWorn[actor->GetBiped1(true).get()][item->formID&0xFFFFFFFF]);
                            
                                if ((p.first & 0xFFFFFFFF) == (item->formID & 0xFFFFFFFF)) {
                                    biped_clear_3d(p.second, 0, 0);
                                    done = true;
                                    found = true;
                                }
                            }
                        }
                    }
                    
                }
            }
        }
    }
    
        
    
    
}
void EquipArmorStuff(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4,
                                                     uint64_t arg5) {

    orig_equiparmorstuff(arg1, arg2, arg3, arg4, arg5);
}
void NewEquipBiped(RE::BipedAnim *bipedanim, uint64_t param_2, uint64_t param_3, uint64_t param_4, uint64_t param_5) {
    std::lock_guard<std::recursive_mutex> lock(g_bipedstate_mutex);
    //unequip_mode = true;

    return orig_equipbiped_fn(bipedanim, param_2, param_3, param_4,
                              param_5);
    
}
void fake_unequip(uint64_t arg1, RE::Actor* actor, RE::TESBoundObject * item, uint64_t arg4, uint64_t arg5, uint64_t arg6,
                  uint64_t arg7, uint64_t arg8, uint64_t arg9, uint64_t arg10, uint64_t arg11) {
    std::lock_guard<std::recursive_mutex> lock(g_bipedstate_mutex);

}
uint64_t  NewAddWornItem(RE::Actor *actor, RE::TESBoundObject *item, int32_t count, uint64_t arg3, uint64_t arg4,
                     uint64_t arg5) {
    
    std::lock_guard<std::recursive_mutex> lock(g_bipedstate_mutex);
    
    if (!item) {
        return 0;
    }
    
    uint64_t retval = 0;
   
    bool no1P = false;
	if (actor)
	{
		uint64_t* actor_raw = (uint64_t*)actor;
		if ((actor_raw[biped_3p_offset / 8] != actor_raw[biped_1p_offset / 8]) && actor_raw[biped_3p_offset / 8] != 0 && actor_raw[biped_1p_offset / 8] != 0) {
			no1P = true;
		}
	}
    if (no1P == false) {
        if (actor->GetBiped1(true)) {
            RE::BipedAnim *bipedanim = actor->GetBiped1(true).get();
            if (!BipedAnimToExtraWorn.contains(actor->GetBiped1(true).get())) {
                BipedAnimToExtraWorn.insert_or_assign(bipedanim, std::map<int, RE::BipedAnim *>());
            }
            bool found = false;
            if (BipedAnimToExtraWorn[actor->GetBiped1(true).get()].contains(item->formID)) {
                found = true;
            }
            
            if (found == false) {
                RE::BipedAnim *(*construct_biped)(RE::BipedAnim *, RE::Actor *, RE::NiNode *) =
                    (RE::BipedAnim * (*)(RE::BipedAnim *, RE::Actor *, RE::NiNode *))(REL::Offset(construct_biped_offset).address());
                RE::BipedAnim *new_biped = (RE::BipedAnim *)malloc(0x2778);
                construct_biped(new_biped, bipedanim->actorRef.get().get()->As<RE::Actor>(), bipedanim->root);
                new_biped->IncRef();
                BipedAnimToExtraWorn[bipedanim].insert(
                    std::pair(item->formID & 0xFFFFFFFF, (RE::BipedAnim *)new_biped));
            }
            std::pair p(item->formID, BipedAnimToExtraWorn[bipedanim][item->formID]);
            {
                if ((p.first & 0xFFFFFFFF) == (item->formID & 0xFFFFFFFF)) {
                    bool done2 = false;
                    bipedanim->IncRef();
                    uint64_t *actor_raw = (uint64_t *)bipedanim->actorRef.get().get();
                    if (actor_raw[biped_1p_offset / 8] != 0x0 && actor_raw[biped_1p_offset / 8] == (uint64_t)bipedanim) {
                        if (item->IsArmor()) {
                            
                            if (actor->GetActorBase()) {
                                actor->IncRefCount();
                                item->IncRef();
                                //SKSE::GetTaskInterface()->AddTask([item,actor]() {
                                    if (actor) {
                                        if (!actor->IsDisabled()) {
											RE::BSTSmartPointer bipedptr((RE::BipedAnim*)actor_raw[biped_1p_offset / 8]);
											PrepareEquipBiped(
                                                (RE::TESObjectARMO *)item, actor->GetRace(), &bipedptr,
                                                (uint64_t)actor->GetActorBase()->actorData.actorBaseFlags.get() & 1);
                                        }
                                    }
                                    actor->DecRefCount();
                                    item->DecRef();
                                //});
                            }
                        }
                    }
                }
            }
        }
    }
    if (actor->GetBiped1(false)) {
        RE::BipedAnim *bipedanim = actor->GetBiped1(false).get();
        if (!BipedAnimToExtraWorn.contains(actor->GetBiped1(false).get())) {
            BipedAnimToExtraWorn.insert_or_assign(bipedanim, std::map<int, RE::BipedAnim *>());
        }
        bool found = false;
        if (BipedAnimToExtraWorn[actor->GetBiped1(false).get()].contains(item->formID)) {
            found=true;
        }

        if (found == false) {
            RE::BipedAnim *(*construct_biped)(RE::BipedAnim *, RE::Actor *, RE::NiNode *) =
                (RE::BipedAnim * (*)(RE::BipedAnim *, RE::Actor *, RE::NiNode *))(REL::Offset(construct_biped_offset).address());
            RE::BipedAnim *new_biped = (RE::BipedAnim *)malloc(0x2778);
            construct_biped(new_biped, bipedanim->actorRef.get().get()->As<RE::Actor>(), bipedanim->root);
            new_biped->IncRef();
            BipedAnimToExtraWorn[bipedanim].insert(std::pair(item->formID & 0xFFFFFFFF, (RE::BipedAnim *)new_biped));
        }
        std::pair p(item->formID & 0xFFFFFFFF,
                    BipedAnimToExtraWorn[actor->GetBiped1(false).get()][item->formID & 0xFFFFFFFF]);
        {
            if ((p.first & 0xFFFFFFFF) == (item->formID & 0xFFFFFFFF)) {
                uint64_t *actor_raw = (uint64_t *)bipedanim->actorRef.get().get();
                /* actor_raw[biped_3p_offset / 8] = (uint64_t)p.second;
                uint64_t Biped1P = actor_raw[biped_1p_offset / 8];
                if (no1P == false) {
                    if (BipedAnimToExtraWorn[actor->GetBiped1(true).get()].contains(item->formID & 0xFFFFFFFF)) {
                        actor_raw[biped_1p_offset / 8] = (uint64_t)BipedAnimToExtraWorn[actor->GetBiped1(true).get()][item->formID];
                    }
                }*/
                retval |= orig_addwornitem_fn(actor, item, count, arg3, arg4, arg5);
                /* if (no1P == false) {
                    actor_raw[biped_1p_offset / 8] = (uint64_t)Biped1P;
                }
                actor_raw[biped_3p_offset / 8] = (uint64_t)bipedanim;*/
                if (item->IsArmor()) {
                    
                    if (actor->GetActorBase()) {
                        actor->IncRefCount();
                        item->IncRef();
                        //SKSE::GetTaskInterface()->AddTask([item, actor]() {
                            if (!actor->IsDisabled()) {
							    RE::BSTSmartPointer bipedptr((RE::BipedAnim*)actor_raw[biped_3p_offset / 8]);
                                
                                PrepareEquipBiped((RE::TESObjectARMO*)item, actor->GetRace(), &bipedptr,
                                                  (uint64_t)actor->GetActorBase()->actorData.actorBaseFlags.get() & 1);
                                                  
                            }
                            actor->DecRefCount();
                            item->DecRef();
                        //});
                    }
                }
                if (no1P == true) {

                }
            }
        }
    }

   
    if (actor) {
        
    }

    return retval;
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	    InitializeLog();
	    logger::info("Loaded plugin {} {}", Plugin::NAME, Plugin::VERSION.string());
	    SKSE::Init(a_skse);
	    SKSE::AllocTrampoline(512);
	    uint8_t *slotpatch_ptr = (uint8_t *)(REL::Offset(slotpatch_offset).address());
	    DWORD oldProtect = 0;
	
	    
	
	
	    
	    uintptr_t* AddWornItemVtable = (uintptr_t*)(REL::Offset(addwornitemvtable0).address());
	    auto &trampoline = SKSE::GetTrampoline();
	    orig_addwornitem_fn =
	        (uint64_t(*)(RE::Actor *, RE::TESBoundObject *, int32_t, uint64_t, uint64_t, uint64_t))(* AddWornItemVtable);
	    VirtualProtect((void *)AddWornItemVtable, 8, PAGE_EXECUTE_READWRITE, &oldProtect);
	    *AddWornItemVtable = (uintptr_t)NewAddWornItem;
		uintptr_t* AddWornItemVtable2 = (uintptr_t*)(REL::Offset(addwornitemvtable1).address());
	    orig_addwornitem_fn =
	        (uint64_t(*)(RE::Actor *, RE::TESBoundObject *, int32_t, uint64_t, uint64_t, uint64_t))(*AddWornItemVtable2);
	    VirtualProtect((void *)AddWornItemVtable2, 8, PAGE_EXECUTE_READWRITE, &oldProtect);
	    *AddWornItemVtable2 = (uintptr_t)NewAddWornItem;
		uintptr_t* AddWornItemVtable3 = (uintptr_t*)(REL::Offset(addwornitemvtable2).address());
	    orig_addwornitem_fn =
	        (uint64_t(*)(RE::Actor *, RE::TESBoundObject *, int32_t, uint64_t, uint64_t, uint64_t))(*AddWornItemVtable3);
	    VirtualProtect((void *)AddWornItemVtable3, 8, PAGE_EXECUTE_READWRITE, &oldProtect);
	    *AddWornItemVtable3 = (uintptr_t)NewAddWornItem;
		for (uint64_t offset : addwornitemhooks) {
			trampoline.write_call<5>(REL::Offset(offset).address(), NewAddWornItem);
		}

	    VirtualProtect((void *)slotpatch_ptr, 6, PAGE_EXECUTE_READWRITE, &oldProtect);
	    slotpatch_ptr[0x0] = 0x48;
	    slotpatch_ptr[0x1] = 0xe9;
#ifdef FOR_VR_1_4_150
		VirtualProtect((void*)REL::Offset(jmp_from_bipedanim_dtor_offset_vr).address(), 12, PAGE_EXECUTE_READWRITE, &oldProtect);
		uint8_t* ret_from_bipedanim_dtor = (uint8_t*)REL::Offset(jmp_from_bipedanim_dtor_offset_vr).address();
		orig_unequip_all_fn = (void (*)(RE::BipedAnim*, uint64_t, uint64_t))(REL::Offset(unequip_all_offset).address());
		trampoline.write_branch<5>((REL::Offset(jmp_from_bipedanim_dtor_offset_vr).address()), HookAfterBipedDtor);

		trampoline.write_call<5>((REL::Offset(call_unequipall_from_dtor_offset).address()), UnequipAllBipedDtor);
#else
		VirtualProtect((void*)REL::Offset(ret_from_bipedanim_dtor_offset).address(), 12, PAGE_EXECUTE_READWRITE, &oldProtect);
		uint8_t* ret_from_bipedanim_dtor = (uint8_t*)REL::Offset(ret_from_bipedanim_dtor_offset).address();
	    ret_from_bipedanim_dtor[1] = 0xe8;
	    orig_unequip_all_fn = (void (*)(RE::BipedAnim *, uint64_t, uint64_t))(REL::Offset(unequip_all_offset).address());
		trampoline.write_branch<5>((REL::Offset(ret_from_bipedanim_dtor_offset+1).address()), HookAfterBipedDtor);

		trampoline.write_call<5>((REL::Offset(call_unequipall_from_dtor_offset).address()), UnequipAllBipedDtor);
	    ret_from_bipedanim_dtor[0] = 0x90;
#endif
	    orig_preparebiped_fn =
	        (void (*)(RE::TESObjectARMO * armor, RE::TESRace * race, RE::BSTSmartPointer<RE::BipedAnim> * bipedanim_sptr,
	                  uint64_t param_4))(REL::Offset(init_worn_armor).address());
		
		trampoline.write_call<5>(REL::Offset(prepare_equip_biped_hook0).address(), PrepareEquipBiped);
		trampoline.write_call<5>(REL::Offset(prepare_equip_biped_hook1).address(), PrepareEquipBiped);
		trampoline.write_call<5>(REL::Offset(prepare_equip_biped_hook2).address(), PrepareEquipBiped);
		trampoline.write_call<5>(REL::Offset(prepare_equip_biped_hook3).address(), PrepareEquipBiped);
	    real_unequip_fn = (void (*)(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5
	                                ))(REL::Offset(real_unequip).address());
	    trampoline.write_call<5>(REL::Offset(unequip_hook).address(),UnequipHook);
		for (uint64_t offset : clear_3d_hooks) {
			trampoline.write_call<5>(REL::Offset(offset).address(), Clear3DHook);
		}
        
	    return true;
}

extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []() noexcept {
	SKSE::PluginVersionData v;
	v.PluginName(Plugin::NAME.data());
	v.PluginVersion(Plugin::VERSION);
	v.UsesAddressLibrary(true);
	v.HasNoStructUse();
	return v;
}();

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface*, SKSE::PluginInfo* pluginInfo)
{
	pluginInfo->name = SKSEPlugin_Version.pluginName;
	pluginInfo->infoVersion = SKSE::PluginInfo::kVersion;
	pluginInfo->version = SKSEPlugin_Version.pluginVersion;
	return true;
}
