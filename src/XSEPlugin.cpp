#pragma warning(disable: 4100 4189)
#include "RE/N/NiSmartPointer.h"
#include "REL/Relocation.h"
#include "SKSE/API.h"
#include "SKSE/Interfaces.h"
#include <Psapi.h>
#include <detours/detours.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <windows.h>
using json = nlohmann::json;

#undef GetObject
#define DLLEXPORT __declspec(dllexport)
//#define FOR640
#define FOR1170
//#define FOR1170
#ifdef FOR640
static uint64_t slotpatch_offset = 0x646d1b;
#endif
#ifdef FOR1170
static uint64_t slotpatch_offset = 0x6a0b0b;
#endif
#ifdef FOR_VR_1_4_150
static uint64_t slotpatch_offset = 0x61565b;
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
static uint64_t clear_3d_hooks[] = { 0x01d2f92, 0x01d41ca, 0x01daf54, 0x062d2dc, 0x062d2f7, 0x06cd14e, 0x06cd1b8, 0x06cd206, 0x08f3d18, 0x08f3d87, 0x09ae34f };
static uint64_t unequip_hook = 0x6703c9;
static uint64_t real_unequip = 0x672a10;
static uint64_t equip_biped = 0x1d40a0;
static uint64_t construct_biped_offset = 0x1d2e60;
static uint64_t biped_dtor = 0x1d2f80;
static uint64_t unequip_biped = 0x1d3250;
static uint64_t init_worn_armor_addon = 0x237b00;
static uint64_t addwornitem = 0x646cb0;
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
static uint64_t init_worn_armor_addon = 0x2775d0;
static uint64_t prepare_equip_biped_hook0 = 0x3be42e;
static uint64_t prepare_equip_biped_hook1 = 0x3bd368;
static uint64_t prepare_equip_biped_hook2 = 0x3bd2c2;
static uint64_t prepare_equip_biped_hook3 = 0x2131b9;
static uint64_t clear_3d_hooks[] = { 0x212472, 0x2136aa, 0x21a434, 0x6870cc, 0x6870e7, 0x726f7e, 0x726fe8, 0x727036, 0x9525f8, 0x952667, 0xa0c76f };
static uint64_t unequip_hook = 0x6ca1c9;
static uint64_t real_unequip = 0x6cc810;
static uint64_t equip_biped = 0x213580;
static uint64_t construct_biped_offset = 0x212340;
static uint64_t biped_dtor = 0x212460;
static uint64_t unequip_biped = 0x212730;
static uint64_t addwornitem = 0x6a0aa0;
static uint64_t OriginalUpdateOffset = 0x667d40;
static uint64_t addwornitem_offset0 = 0x6a0e5e;
static uint64_t addwornitem_offset1 = 0x6a0c6e;
static uint64_t biped_inventory_update_offset = 0x3bba20;
static uint64_t EquipArmor_offset = 0x3bcfc0;
#endif
#ifdef FOR_VR_1_4_150
static uint64_t biped_1p_offset = 0xfe8;
static uint64_t biped_3p_offset = 0x260;
static uint64_t addwornitemvtable0 = 0x16ceb40;
static uint64_t addwornitemvtable1 = 0x16d7098;
static uint64_t addwornitemvtable2 = 0x16e24e8;
static uint64_t addwornitemhooks[] = { 0x1d6212, 0x1d7024, 0x1d73c1, 0x1de722, 0x5fcbcd, 0x5fcbe8, 0x69c77e, 0x69c7e6, 0x69c834, 0x8df251, 0x8df2be, 0x9acc3f };
static uint64_t jmp_from_bipedanim_dtor_offset_vr = 0x1d624e;
static uint64_t call_unequipall_from_dtor_offset = 0x1d6212;
static uint64_t unequip_all_offset = 0x1d6530;
static uint64_t init_worn_armor = 0x239e00;
static uint64_t prepare_equip_biped_hook0 = 0x1d7003;
static uint64_t prepare_equip_biped_hook1 = 0x373fb2;
static uint64_t prepare_equip_biped_hook2 = 0x374058;
static uint64_t prepare_equip_biped_hook3 = 0x374e4e;
static uint64_t clear_3d_hooks[] = { 0x1d7024, 0x1d73c1, 0x1de722, 0x5fcbcd, 0x5fcbe8, 0x69c77e, 0x69c7e6, 0x69c834 };
static uint64_t unequip_hook = 0x6412d8;
static uint64_t real_unequip = 0x6436c0;
static uint64_t equip_biped = 0x1d7450;
static uint64_t construct_biped_offset = 0x1d60f0;
static uint64_t biped_dtor = 0x1d6200;
static uint64_t eh_vector_destructor_iterator_offset = 0x138aa7c;
static uint64_t unequip_biped = 0x1d6690;
static uint64_t init_worn_armor_addon = 0x2383a0;
static uint64_t addwornitem = 0x6155f0;

#endif
std::map<RE::Actor* ,std::set<RE::FormID>> ExtraWornAddons;
void* OriginalUpdatePtr = nullptr;
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
int allow_unlimited = 0;
std::set<RE::FormID> VirtualSlots;
std::map<RE::Actor*, std::map<RE::FormID, RE::TESBoundObject*>> ActorToVirtualSlotEquipment;
std::map<RE::BipedAnim*, std::map<int, RE::BipedAnim*>> BipedAnimToExtraWorn;
std::map<RE::Actor*, std::vector<uint32_t>> ExtraWornSlotMasks;
std::recursive_mutex g_bipedstate_mutex;
static std::recursive_mutex g_skee_mutex;
void (*orig_equiparmorstuff)(uint64_t arg1, uint64_t arg2, uint64_t arg3,
	uint64_t arg4, uint64_t arg5) = nullptr;
uint64_t (*orig_equiparmor)(RE::TESActorBase* actor, uint64_t arg2, RE::BSTSmartPointer<RE::BipedAnim>* bipedanim_sptr, RE::TESObjectARMO**) = nullptr;
uint64_t (*orig_addwornitem_fn)(RE::Actor* actor, RE::TESBoundObject* item, int32_t count, uint64_t forceEquip, uint64_t arg4,
	uint64_t arg5) = nullptr;
uint64_t (*real_unequip_fn)(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5) = nullptr;
uint64_t (*orig_unequip_fn)(RE::BipedAnim*, RE::BIPOBJECT*, char, uint64_t) = nullptr;
RE::BipedAnim* (*BipedAnimconstruct)(RE::BipedAnim*, void*, void*) = nullptr;
void UnequipBipedHook(RE::BipedAnim* anim, RE::BIPOBJECT* obj, uint64_t arg3, uint64_t arg4, uint64_t arg5);
void (*orig_equipbiped_fn)(RE::BipedAnim*, uint64_t, uint64_t, uint64_t, uint64_t) = nullptr;
void (*orig_init_worn_armor_addon_fn)(RE::TESObjectARMA* aa, RE::TESObjectARMO* armor,
	RE::BSTSmartPointer<RE::BipedAnim>* bipedanim_sptr, uint64_t param_4) = nullptr;
void (*orig_init_worn_armor_fn)(RE::TESObjectARMO* armor, RE::TESRace* race,
	RE::BSTSmartPointer<RE::BipedAnim>* bipedanim_sptr, uint64_t param_4) = nullptr;
auto unequip_biped_fn = (void (*)(RE::BipedAnim*, RE::BIPOBJECT*, uint64_t, uint64_t, uint64_t)) nullptr;
static std::atomic<uint32_t> skee_loaded = 0;
static bool unequip_mode = true;
void (*orig_unequip_all_fn)(RE::BipedAnim*, uint64_t, uint64_t) = nullptr;
bool (*orig_update_3d_hook_fn)(RE::Actor* Actor) = nullptr;
void Clear3DHook(RE::BipedAnim* bipedanim, uint64_t arg2, uint64_t arg3);
static RE::TESObjectARMO* current_prepared_armor = nullptr;
static RE::BipedAnim* to_destroy_bipedanim = nullptr;
void (*skee64_Biped1Original)(RE::Actor* actor, void* callback) = nullptr;
void UnequipAllBipedDtor(RE::BipedAnim* bipedanim, uint64_t arg2, uint64_t arg3)
{
	std::lock_guard<std::recursive_mutex> lock(g_bipedstate_mutex);
	Clear3DHook(bipedanim, arg2, arg3);
	if (to_destroy_bipedanim == nullptr) {
		to_destroy_bipedanim = bipedanim;
	}
}
bool InsideExtraDestroy = false;

void HookAfterBipedDtor(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4)
{
	std::lock_guard<std::recursive_mutex> lock(g_bipedstate_mutex);
#ifdef FOR_VR_1_4_150
	auto eh_vector_destructor_iterator = (void (*)(uint64_t, uint64_t, uint64_t, uint64_t))(REL::Offset(eh_vector_destructor_iterator_offset).address());
	eh_vector_destructor_iterator(arg1, arg2, arg3, arg4);

#endif
	if (to_destroy_bipedanim != nullptr) {
		if (InsideExtraDestroy == true) {
			return;
		}
		if (BipedAnimToExtraWorn.contains(to_destroy_bipedanim)) {
			InsideExtraDestroy = true;

			for (auto& p : BipedAnimToExtraWorn[to_destroy_bipedanim]) {
				auto biped_dtor_3d = (void (*)(RE::BipedAnim*, uint64_t, uint64_t))(REL::Offset(biped_dtor).address());
				auto biped_clear_3d =
					(void (*)(RE::BipedAnim*, uint64_t, uint64_t))(REL::Offset(unequip_all_offset).address());
				if (p.second != nullptr) {
					biped_dtor_3d(p.second, arg2, arg3);
					free((void*)p.second);
				}
				p.second = nullptr;
			}
			BipedAnimToExtraWorn.erase(to_destroy_bipedanim);
			InsideExtraDestroy = false;
		}

		to_destroy_bipedanim = nullptr;
	}
}
void Clear3DHook(RE::BipedAnim* bipedanim, uint64_t arg2, uint64_t arg3)
{
	std::lock_guard<std::recursive_mutex> lock(g_bipedstate_mutex);
	auto biped_clear_3d =
		(void (*)(RE::BipedAnim*, uint64_t, uint64_t))(REL::Offset(unequip_all_offset).address());
	if (bipedanim == nullptr) {
		return;
	}
	for (int i = 0; i < 0x2a; i++) {
		UnequipBipedHook(bipedanim, &bipedanim->objects[i], 1, 0, 0);
	}
	for (int i = 0; i < 0x2a; i++) {
		UnequipBipedHook(bipedanim, &bipedanim->bufferedObjects[i], 1, 0, 0);
	}
	biped_clear_3d(bipedanim, arg2, arg3);
	if (BipedAnimToExtraWorn.contains(bipedanim)) {
		for (auto p : BipedAnimToExtraWorn[bipedanim]) {
			if (p.second != nullptr) {
				for (int i = 0; i < 0x2a; i++) {
					UnequipBipedHook(p.second, &p.second->objects[i], 1, 0, 0);
				}
				for (int i = 0; i < 0x2a; i++) {
					UnequipBipedHook(p.second, &p.second->bufferedObjects[i], 1, 0, 0);
				}
			}
		}
	}
}
auto bipedVector3P = std::vector<RE::BipedAnim*>();
auto bipedVector1P = std::vector<RE::BipedAnim*>();
uint64_t bipedvector_idx = 0;
RE::BSTSmartPointer<RE::BipedAnim>& (*GetBiped1_fn)(RE::Actor* actor, bool firstperson) = (RE::BSTSmartPointer<RE::BipedAnim> & (*)(RE::Actor * actor, bool firstperson)) nullptr;
static uint8_t skee_nop_opcodes[0x1d];
uintptr_t skee_nop_address = 0x0;
const RE::BSTSmartPointer<RE::BipedAnim>* skee64_GetBiped1_ERRORS_ABOVE_THIS_CALL_ARE_ArmorUnlimited_Errors_DO_NOT_REPORT_AS_RACEMENU_ERRORS_WITHOUT_ASKING_ArmorUnlimited_developers_first(RE::Actor* actor, bool firstperson)
{
	std::lock_guard<std::recursive_mutex> lock(g_bipedstate_mutex);
	if (bipedvector_idx < bipedVector3P.size() && firstperson == false) {
		return new RE::BSTSmartPointer<RE::BipedAnim>(bipedVector3P[bipedvector_idx]);
	}
	if (bipedvector_idx < bipedVector1P.size() && firstperson == true) {
		return new RE::BSTSmartPointer<RE::BipedAnim>(bipedVector1P[bipedvector_idx]);
	}

	return nullptr;
}
void skee64_Biped1Hook_ERRORS_ABOVE_THIS_CALL_ARE_ArmorUnlimited_Errors_DO_NOT_REPORT_AS_RACEMENU_ERRORS_WITHOUT_ASKING_ArmorUnlimited_developers_first(RE::Actor* actor, void* callback)
{
	std::lock_guard<std::recursive_mutex> lock(g_bipedstate_mutex);
	if (actor != nullptr) {
		if (GetBiped1_fn == nullptr) {
			uintptr_t** vtable_ptr = (uintptr_t**)actor;
			uintptr_t* vtable = *vtable_ptr;
			GetBiped1_fn = (RE::BSTSmartPointer<RE::BipedAnim> & (*)(RE::Actor * actor, bool firstperson)) vtable[0x7f];
		}

		bipedVector1P.clear();
		bipedVector3P.clear();
		bipedvector_idx = 0;
		bipedVector3P.push_back(GetBiped1_fn(actor, false).get());
		bipedVector1P.push_back(GetBiped1_fn(actor, true).get());
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		DetourAttach(&(PVOID&)GetBiped1_fn, &skee64_GetBiped1_ERRORS_ABOVE_THIS_CALL_ARE_ArmorUnlimited_Errors_DO_NOT_REPORT_AS_RACEMENU_ERRORS_WITHOUT_ASKING_ArmorUnlimited_developers_first);
		if (DetourTransactionCommit() != NO_ERROR) {
			return;
		}
		if (BipedAnimToExtraWorn.contains(GetBiped1_fn(actor, false).get())) {
			for (auto p : BipedAnimToExtraWorn[GetBiped1_fn(actor, false).get()]) {
				if (p.second != nullptr) {
					bipedVector3P.push_back(p.second);
				}
			}
		}

		if (BipedAnimToExtraWorn.contains(GetBiped1_fn(actor, true).get())) {
			for (auto p : BipedAnimToExtraWorn[GetBiped1_fn(actor, true).get()]) {
				if (p.second != nullptr) {
					bipedVector1P.push_back(p.second);
				}
			}
		}
		int bpsize = (int)bipedVector3P.size();
		if ((int)bipedVector1P.size() >= bpsize) {
			bpsize = (int)bipedVector1P.size();
		}

		REL::safe_fill(skee_nop_address, 0x90, 0x1d);
		if (bpsize == 0) {
			REL::safe_write((std::uintptr_t)skee_nop_address, (const void*)skee_nop_opcodes, 0x1d);
		}
		for (int i = bpsize - 1; i >= 0; i -= 1) {
			bipedvector_idx = (int)i;
			if (i == 0) {
				REL::safe_write((std::uintptr_t)skee_nop_address, (const void*)skee_nop_opcodes, 0x1d);
			}
			{
				skee64_Biped1Original(actor, callback);
			}
		}

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)GetBiped1_fn, &skee64_GetBiped1_ERRORS_ABOVE_THIS_CALL_ARE_ArmorUnlimited_Errors_DO_NOT_REPORT_AS_RACEMENU_ERRORS_WITHOUT_ASKING_ArmorUnlimited_developers_first);
		if (DetourTransactionCommit() != NO_ERROR) {
			return;
		}
	}
}
bool Update3DHook(RE::Actor* Actor)
{
	std::lock_guard<std::recursive_mutex> lock(g_bipedstate_mutex);
	auto Biped3rd = Actor->GetBiped1(false);
	auto Biped1st = Actor->GetBiped1(true);
#ifdef FOR1170
	auto biped_equip_finish = (void (*)(RE::BipedAnim*, uint64_t, uint64_t, uint64_t, uint64_t))(REL::Offset(equip_biped).address());
#else
	auto biped_equip_finish = (void (*)(RE::BipedAnim*, float, uint64_t))(REL::Offset(equip_biped).address());
#endif
	if (BipedAnimToExtraWorn.contains(Biped3rd.get())) {
		for (auto ew : BipedAnimToExtraWorn[Biped3rd.get()]) {
			if (ew.second != nullptr) {
				if (ew.second->root == nullptr) {
					ew.second->root = Biped3rd.get()->root;
				}
				for (int i = 0; i < 0x2a; i++) {
					UnequipBipedHook(ew.second, &ew.second->objects[i], 1, 0, 0);
				}
				for (int i = 0; i < 0x2a; i++) {
					UnequipBipedHook(ew.second, &ew.second->bufferedObjects[i], 1, 0, 0);
				}
				if (RE::TESForm* form=RE::TESForm::LookupByID(ew.first)) {
					if (RE::TESObjectARMO* armor = form->As<RE::TESObjectARMO>()) {
						//orig_init_worn_armor_fn(armor, Actor->GetRace(), &Biped3rd, Actor->GetActorBase()->IsFemale() ? 1 : 0);
					}
				}
				
			}
		}
	}
	if (BipedAnimToExtraWorn.contains(Biped1st.get()) && Biped1st != Biped3rd) {
		for (auto ew : BipedAnimToExtraWorn[Biped1st.get()]) {
			if (ew.second != nullptr) {
				if (ew.second->root == nullptr) {
					ew.second->root = Biped1st.get()->root;
				}
				for (int i = 0; i < 0x2a; i++) {
					UnequipBipedHook(ew.second, &ew.second->objects[i], 1, 0, 0);
				}
				for (int i = 0; i < 0x2a; i++) {
					UnequipBipedHook(ew.second, &ew.second->bufferedObjects[i], 1, 0, 0);
				}
				if (RE::TESForm* form = RE::TESForm::LookupByID(ew.first)) {
					if (RE::TESObjectARMO* armor = form->As<RE::TESObjectARMO>()) {
						//orig_init_worn_armor_fn(armor, Actor->GetRace(), &Biped1st, Actor->GetActorBase()->IsFemale() ? 1 : 0);
					}
				}
			}
		}
	}
	if (BipedAnimToExtraWorn.contains(Biped3rd.get())) {
		if (ExtraWornSlotMasks.contains(Actor)) {
			for (int i = 0; i < 0x2a; i++) {
				if (ExtraWornSlotMasks[Actor][i] != 0) {
					UnequipBipedHook(Biped3rd.get(), &Biped3rd.get()->objects[i], 1, 0, 0);
				}
			}
			for (int i = 0; i < 0x2a; i++) {
				if (ExtraWornSlotMasks[Actor][i] != 0) {
					UnequipBipedHook(Biped3rd.get(), &Biped3rd.get()->bufferedObjects[i], 1, 0, 0);
				}
			}
		}
	}
	if (BipedAnimToExtraWorn.contains(Biped1st.get()) && Biped1st != Biped3rd) {
		if (ExtraWornSlotMasks.contains(Actor)) {
			for (int i = 0; i < 0x2a; i++) {
				if (ExtraWornSlotMasks[Actor][i] != 0) {
					UnequipBipedHook(Biped1st.get(), &Biped1st.get()->objects[i], 1, 0, 0);
				}
			}
			for (int i = 0; i < 0x2a; i++) {
				if (ExtraWornSlotMasks[Actor][i] != 0) {
					UnequipBipedHook(Biped1st.get(), &Biped1st.get()->bufferedObjects[i], 1, 0, 0);
				}
			}
		}
	}
	bool retval=orig_update_3d_hook_fn(Actor);
	if (BipedAnimToExtraWorn.contains(Biped3rd.get())) {
		if (ExtraWornSlotMasks.contains(Actor)) {
			
			for (int i = 0; i < 0x2a; i++) {
				if (ExtraWornSlotMasks[Actor][i] != 0) {
					UnequipBipedHook(Biped3rd.get(), &Biped3rd.get()->objects[i], 1, 0, 0);
				}
			}
			for (int i = 0; i < 0x2a; i++) {
				if (ExtraWornSlotMasks[Actor][i] != 0) {
					UnequipBipedHook(Biped3rd.get(), &Biped3rd.get()->bufferedObjects[i], 1, 0, 0);
				}
			}
		}
	}
	if (BipedAnimToExtraWorn.contains(Biped1st.get()) && Biped1st != Biped3rd) {
		if (ExtraWornSlotMasks.contains(Actor)) {
			for (int i = 0; i < 0x2a; i++) {
				if (ExtraWornSlotMasks[Actor][i] != 0) {
					UnequipBipedHook(Biped1st.get(), &Biped1st.get()->objects[i], 1, 0, 0);
				}
			}
			for (int i = 0; i < 0x2a; i++) {
				if (ExtraWornSlotMasks[Actor][i] != 0) {
					UnequipBipedHook(Biped1st.get(), &Biped1st.get()->bufferedObjects[i], 1, 0, 0);
				}
			}
		}
	}
	return retval;
}
uint64_t EquipArmorHook(RE::TESActorBase* actorBase, uint64_t arg2, RE::BSTSmartPointer<RE::BipedAnim>* bipedanim_sptr,RE::TESObjectARMO** ItemPtrPtr)
{
	std::lock_guard<std::recursive_mutex> lock(g_bipedstate_mutex);
	if (ItemPtrPtr != nullptr) {
		RE::TESObjectARMO* ItemPtr = *ItemPtrPtr;
		if (bipedanim_sptr != nullptr && bipedanim_sptr->get() != nullptr) {
			if (BipedAnimToExtraWorn.contains(bipedanim_sptr->get())) {
				if (ItemPtr && ItemPtr->formID && ItemPtr->formType == RE::FormType::Armor) {
					if (BipedAnimToExtraWorn[bipedanim_sptr->get()].contains(ItemPtr->formID)) {
						Clear3DHook(BipedAnimToExtraWorn[bipedanim_sptr->get()][ItemPtr->formID], 1, 0);
					}
				}
			}
		}
	}
#ifdef FOR1170
	auto biped_equip_finish = (void (*)(RE::BipedAnim*, uint64_t, uint64_t, uint64_t, uint64_t))(REL::Offset(equip_biped).address());
#else
	auto biped_equip_finish = (void (*)(RE::BipedAnim*, float, uint64_t))(REL::Offset(equip_biped).address());
#endif
	uint64_t retval=orig_equiparmor(actorBase,arg2,bipedanim_sptr,ItemPtrPtr);
	if (ItemPtrPtr != nullptr) {
		RE::TESObjectARMO* ItemPtr = *ItemPtrPtr;
		if (bipedanim_sptr != nullptr && bipedanim_sptr->get() != nullptr) {
			if (BipedAnimToExtraWorn.contains(bipedanim_sptr->get())) {
				if (ItemPtr && ItemPtr->formID && ItemPtr->formType == RE::FormType::Armor) {
					if (BipedAnimToExtraWorn[bipedanim_sptr->get()].contains(ItemPtr->formID)) {
						if (BipedAnimToExtraWorn[bipedanim_sptr->get()][ItemPtr->formID] != nullptr) {
							biped_equip_finish(BipedAnimToExtraWorn[bipedanim_sptr->get()][ItemPtr->formID], BipedAnimToExtraWorn[bipedanim_sptr->get()][ItemPtr->formID]->actorRef.get()->As<RE::Actor>()->GetActorBase()->IsFemale(), 0, 0, 0);
						}
					}
				}
			}
		}
	}
	/*
	if (bipedanim_sptr != nullptr && bipedanim_sptr->get() != nullptr && bipedanim_sptr->get()->actorRef.get() != nullptr && bipedanim_sptr->get()->actorRef.get()->As<RE::Actor>() != nullptr) {
		auto actor_handle = bipedanim_sptr->get()->actorRef.get()->As<RE::Actor>()->GetHandle();
		SKSE::GetTaskInterface()->AddTask([actor_handle]() {
			if (actor_handle.get() && actor_handle.get().get() != nullptr && actor_handle.get().get()->Is3DLoaded()) {
				Update3DHook(actor_handle.get().get());
			}
		});
	}*/
	return retval;
}
void InitWornArmorAddonHook(RE::TESObjectARMA* aa_new, RE::TESObjectARMO* armor, RE::BSTSmartPointer<RE::BipedAnim>* bipedanim_sptr,
	uint64_t param_4)
{
	std::lock_guard<std::recursive_mutex> lock(g_bipedstate_mutex);
	if (HMODULE handle = GetModuleHandleA("skee64")) {
		uint32_t expected = 0;
		if (skee_loaded.compare_exchange_strong(expected, 1) == true && expected == 0) {
			MODULEINFO skee64_info;
			GetModuleInformation(GetCurrentProcess(), handle, &skee64_info, sizeof(skee64_info));

			uint8_t signature[] = { 0xff, 0x90, 0xf0, 0x03, 0x00, 0x00 };
			if (memcmp(signature, (void*)((uintptr_t)skee64_info.lpBaseOfDll + (uintptr_t)0xc2950 + (uintptr_t)0x28), sizeof(signature)) == 0) {
				skee64_Biped1Original = (void (*)(RE::Actor* actor, void* callback))((uintptr_t)skee64_info.lpBaseOfDll + (uintptr_t)0xc2950);
				skee_nop_address = ((uintptr_t)skee64_info.lpBaseOfDll + (uintptr_t)0xc2ac9);
				memcpy(skee_nop_opcodes, (uint8_t*)((uintptr_t)skee64_info.lpBaseOfDll + (uintptr_t)0xc2ac9), 0x1d);
				DetourTransactionBegin();
				DetourUpdateThread(GetCurrentThread());
				DetourAttach(&(PVOID&)skee64_Biped1Original, &skee64_Biped1Hook_ERRORS_ABOVE_THIS_CALL_ARE_ArmorUnlimited_Errors_DO_NOT_REPORT_AS_RACEMENU_ERRORS_WITHOUT_ASKING_ArmorUnlimited_developers_first);
				DetourTransactionCommit();
			}
		}
	}

	if (bipedanim_sptr != nullptr && bipedanim_sptr->get() != nullptr) {
		if (!ExtraWornSlotMasks.contains(bipedanim_sptr->get()->actorRef.get().get()->As<RE::Actor>())) {
			ExtraWornSlotMasks.insert(std::pair(bipedanim_sptr->get()->actorRef.get().get()->As<RE::Actor>(), std::vector<uint32_t>(0x2a)));
		}
		if (bipedanim_sptr->get()->actorRef.get().get()->As<RE::Actor>()->GetSkin() == armor) {
			orig_init_worn_armor_addon_fn(aa_new, armor, bipedanim_sptr, param_4);
			if (ExtraWornSlotMasks.contains(bipedanim_sptr->get()->actorRef.get().get()->As<RE::Actor>())) {
				for (uint32_t i = 0x0; i < 0x2a; i++) {
					if ((ExtraWornSlotMasks[bipedanim_sptr->get()->actorRef.get().get()->As<RE::Actor>()][i]) != 0) {
						
							//unequip_biped_fn(bipedanim_sptr->get(), &bipedanim_sptr->get()->objects[i], 1, 0, 0);
							//unequip_biped_fn(bipedanim_sptr->get(), &bipedanim_sptr->get()->bufferedObjects[i], 1, 0, 0);
						
					}
				}
			}
			return;
		}

#ifdef FOR1170
		auto biped_equip_finish = (void (*)(RE::BipedAnim*, uint64_t, uint64_t, uint64_t, uint64_t))(REL::Offset(equip_biped).address());
#else
		auto biped_equip_finish = (void (*)(RE::BipedAnim*, float, uint64_t))(REL::Offset(equip_biped).address());
#endif
		auto biped_clear_3d =
			(void (*)(RE::BipedAnim*, uint64_t, uint64_t))(REL::Offset(unequip_all_offset).address());
		RE::BipedAnim* bipedanim = bipedanim_sptr->get();
		RE::BipedAnim* new_biped = nullptr;
		if ((bipedanim->actorRef.get()).get() && bipedanim->root != nullptr &&
			bipedanim->actorRef.get().get()->formType == RE::FormType::ActorCharacter) {
			if (!BipedAnimToExtraWorn.contains(bipedanim)) {
				BipedAnimToExtraWorn.insert_or_assign(bipedanim, std::map<int, RE::BipedAnim*>());
			}
			RE::Actor* actor = bipedanim->actorRef.get().get()->As<RE::Actor>();
			if (!ActorToVirtualSlotEquipment.contains(actor)) {
				ActorToVirtualSlotEquipment.insert(std::pair(actor, std::map<RE::FormID, RE::TESBoundObject*>()));
			}
			if (!ExtraWornSlotMasks.contains(bipedanim_sptr->get()->actorRef.get().get()->As<RE::Actor>())) {
				ExtraWornSlotMasks.insert(std::pair(bipedanim_sptr->get()->actorRef.get().get()->As<RE::Actor>(), std::vector<uint32_t>(0x2a)));
			}
			if (!ExtraWornAddons.contains(bipedanim_sptr->get()->actorRef.get().get()->As<RE::Actor>())) {
				ExtraWornAddons.insert(std::pair(bipedanim_sptr->get()->actorRef.get().get()->As<RE::Actor>(), std::set<RE::FormID>()));
			}
			
			if (armor != nullptr) {
				if (armor->formType == RE::FormType::Armor) {
					if (aa_new != nullptr) {
						if (!ExtraWornAddons[bipedanim_sptr->get()->actorRef.get().get()->As<RE::Actor>()].contains(aa_new->formID)) {
							for (int slot = 0; slot < 0x2a; slot++) {
								ExtraWornSlotMasks[actor][slot] += (((uint32_t)aa_new->GetSlotMask() & (1 << slot)) != 0x0 ? 1 : 0);
								ExtraWornAddons[bipedanim_sptr->get()->actorRef.get().get()->As<RE::Actor>()].insert(aa_new->formID);
							}
						}
					}
					bool found = false;
					if (BipedAnimToExtraWorn[bipedanim->actorRef.get().get()->GetBiped1(false).get()].contains(
							armor->formID)) {
						found = true;
					}
					if (found == false) {
						RE::BipedAnim* (*construct_biped)(RE::BipedAnim*, RE::Actor*, RE::NiNode*) =
							(RE::BipedAnim *
								(*)(RE::BipedAnim*, RE::Actor*, RE::NiNode*))(REL::Offset(construct_biped_offset).address());
						new_biped = (RE::BipedAnim*)malloc(0x2778);
						construct_biped(new_biped, bipedanim->actorRef.get().get()->As<RE::Actor>(), bipedanim->root);
						new_biped->IncRef();
						new_biped->IncRef();
						BipedAnimToExtraWorn[bipedanim].insert(
							std::pair(armor->formID & 0xFFFFFFFF, (RE::BipedAnim*)new_biped));
					}
					if (BipedAnimToExtraWorn[bipedanim][armor->formID & 0xFFFFFFFF] != nullptr) {
						std::pair p(armor->formID & 0xFFFFFFFF,
							BipedAnimToExtraWorn[bipedanim][armor->formID & 0xFFFFFFFF]);

						if (new_biped == nullptr) {
							new_biped = p.second;
							for (int slot = 0; slot < 0x2a; slot++) {
								if (ExtraWornSlotMasks[actor][slot] > 0) {
									//ExtraWornSlotMasks[actor][slot] -= ((new_biped->objects[slot].addon != nullptr || new_biped->bufferedObjects[slot].addon != nullptr) ? 1 : 0);
								}
								if (new_biped->objects[slot].partClone != nullptr) {
									//new_biped->root->DetachChild(new_biped->objects[slot].partClone.get());
								} else if (new_biped->bufferedObjects[slot].partClone != nullptr) {
									//new_biped->root->DetachChild(new_biped->bufferedObjects[slot].partClone.get());
								}
								
							}
							//biped_clear_3d(p.second, 0, 0);
						}
						/* for (int slot = 0; slot < 0x2a; slot++) {
							if (((uint32_t)aa_new->GetSlotMask() & (1 << slot)) != 0x0) {
								unequip_biped_fn(new_biped, &new_biped->objects[slot], 1, 0, 0);
							}
							if (((uint32_t)aa_new->GetSlotMask() & (1 << slot)) != 0x0) {
								unequip_biped_fn(new_biped, &new_biped->bufferedObjects[slot], 1, 0, 0);
							}
						}*/
						bool done2 = false;
						bipedanim->IncRef();
						bipedanim->IncRef();
						uint64_t* actor_raw = (uint64_t*)bipedanim->actorRef.get().get();
						if ((actor_raw[biped_1p_offset / 8] != actor_raw[biped_3p_offset / 8]) &&
							actor_raw[biped_1p_offset / 8] != 0x0 && actor_raw[biped_1p_offset / 8] == (uint64_t)bipedanim) {
							RE::BSTSmartPointer new_bipedP(p.second);

							orig_init_worn_armor_addon_fn(aa_new, armor, &new_bipedP, param_4);
							/* for (int slot = 0; slot < 32; slot++) {
										ExtraWornSlotMasks[actor][slot] += ((p.second->objects[slot].addon == aa_new || p.second->bufferedObjects[slot].addon == aa_new) ? 1 : 0);
										if (p.second->bufferedObjects[slot].partClone != nullptr) {
											//p.second->root->AttachChild(p.second->bufferedObjects[slot].partClone.get());
										}
									}*/

							std::set<RE::TESObjectARMA*> added_addons;
							for (uint64_t i = 0; i < 0x2a; i++) {
								/*
                                        if (RE::TESObjectARMA *aa = armor->GetArmorAddonByMask(
                                                bipedanim->actorRef.get().get()->As<RE::Actor>()->GetRace(),
                                                (RE::BIPED_MODEL::BipedObjectSlot)(1ULL << i))) {
                                            if (aa != aa_new) {
												continue;
                                            }
											if (bipedanim->objects[i].addon == aa && bipedanim->objects[i].item != nullptr) {
												unequip_biped_fn(bipedanim, &bipedanim->objects[i], 1, 0x0, 0x0);
											}
											if (bipedanim->bufferedObjects[i].addon == aa && bipedanim->bufferedObjects[i].item != nullptr) {
												unequip_biped_fn(bipedanim, &bipedanim->bufferedObjects[i], 1, 0x0, 0x0);
											}
                                            if (added_addons.contains(aa)) {
												continue;
                                            }
                                        
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
											added_addons.insert(aa);
                                        }*/
							}

							done2 = true;
						} else if (actor_raw[biped_3p_offset / 8] != 0x0 &&
								   actor_raw[biped_3p_offset / 8] == (uint64_t)bipedanim) {
							RE::BSTSmartPointer new_bipedP(p.second);

							//orig_preparebiped_fn(armor, race, &new_biped, param_4);
							std::set<RE::TESObjectARMA*> added_addons;
							/*
								for (int i = 0; i < 0x2a; i++) {
									    
                                        if (RE::TESObjectARMA *aa = armor->GetArmorAddonByMask(
                                                bipedanim->actorRef.get().get()->As<RE::Actor>()->GetRace(),
                                                (RE::BIPED_MODEL::BipedObjectSlot)(1ULL << i))) {
										if (aa != aa_new) {
											continue;
										}
										if (bipedanim->objects[i].addon == aa && bipedanim->objects[i].item != nullptr) {
											unequip_biped_fn(bipedanim, &bipedanim->objects[i], 1, 0x0, 0x0);
										}
										if (bipedanim->bufferedObjects[i].addon == aa && bipedanim->bufferedObjects[i].item != nullptr) {
											unequip_biped_fn(bipedanim, &bipedanim->bufferedObjects[i], 1, 0x0, 0x0);
										}
										if (added_addons.contains(aa)) {
											continue;
										}
                                        p.second->objects[i].item = armor;
                                        p.second->objects[i].addon = aa;
                                        p.second->objects[i].part = &aa->bipedModels[param_4];
                                        p.second->objects[i].skinTexture = aa->skinTextures[param_4];
                                        
                                        if (p.second->objects[i].partClone) {
                                            if (p.second->root) {
                                                //p.second->root->AttachChild(p.second->objects[i].partClone.get(), true);
                                            }
                                        }
										added_addons.insert(aa);
                                    }
                                
                                    
                                }*/
							orig_init_worn_armor_addon_fn(aa_new, armor, &new_bipedP, param_4);

							done2 = true;
							//actor_raw[biped_3p_offset / 8] = (uint64_t)bipedanim;
						}

						bipedanim->DecRef();
						bipedanim->DecRef();
						if (done2 == true) {
#ifdef FOR1170
							//biped_equip_finish(new_biped, actor->GetActorBase()->IsFemale(), 0, 0, 0);
#else
							biped_equip_finish(new_biped, actor->GetActorBase()->IsFemale() ? 1.0f:0.0f, 0);
#endif
							if (!ExtraWornSlotMasks.contains(bipedanim_sptr->get()->actorRef.get().get()->As<RE::Actor>())) {
								ExtraWornSlotMasks.insert(std::pair(bipedanim_sptr->get()->actorRef.get().get()->As<RE::Actor>(), std::vector<uint32_t>(0x2a)));
							}
							if (!ExtraWornAddons.contains(bipedanim_sptr->get()->actorRef.get().get()->As<RE::Actor>())) {
								ExtraWornAddons.insert(std::pair(bipedanim_sptr->get()->actorRef.get().get()->As<RE::Actor>(), std::set<RE::FormID>()));
							}
							for (int slot = 0; slot < 0x2a; slot++) {
								if (slot < 32) {
									
								}
								if (new_biped->objects[slot].partClone != nullptr) {
									RE::NiNode* root_node = new_biped->root;
									RE::NiAVObject* object = new_biped->objects[slot].partClone.get();

									//new_biped->root->AttachChild(new_biped->objects[slot].partClone.get());
								} else if (new_biped->bufferedObjects[slot].partClone != nullptr) {
									RE::NiNode* root_node = new_biped->root;
									RE::NiAVObject* object = new_biped->bufferedObjects[slot].partClone.get();
									//new_biped->root->AttachChild(new_biped->bufferedObjects[slot].partClone.get());
								}
							}
							
							for (uint32_t i = 0x0; i < 0x2a; i++) {
								if ((ExtraWornSlotMasks[bipedanim_sptr->get()->actorRef.get().get()->As<RE::Actor>()][i]) != 0) {
									if (bipedanim_sptr->get()->objects[i].partClone != nullptr) {
										//unequip_biped_fn(bipedanim_sptr->get(), &bipedanim_sptr->get()->objects[i], 1, 0, 0);
									}
									if (bipedanim_sptr->get()->bufferedObjects[i].partClone != nullptr) {
										//unequip_biped_fn(bipedanim_sptr->get(), &bipedanim_sptr->get()->bufferedObjects[i], 1, 0, 0);
									}
								}
							}
							return;
						}
					}
				}
				
			}
		}
	}

	orig_init_worn_armor_addon_fn(aa_new, armor, bipedanim_sptr, param_4);
	if (!ExtraWornSlotMasks.contains(bipedanim_sptr->get()->actorRef.get().get()->As<RE::Actor>())) {
		ExtraWornSlotMasks.insert(std::pair(bipedanim_sptr->get()->actorRef.get().get()->As<RE::Actor>(), std::vector<uint32_t>(0x2a)));
	}
	
	for (uint32_t i = 0x0; i < 0x2a; i++) {
		if ((ExtraWornSlotMasks[bipedanim_sptr->get()->actorRef.get().get()->As<RE::Actor>()][i]) != 0) {
			if (bipedanim_sptr->get()->objects[i].partClone != nullptr) {
				//unequip_biped_fn(bipedanim_sptr->get(), &bipedanim_sptr->get()->objects[i], 1, 0, 0);
			}
			if (bipedanim_sptr->get()->bufferedObjects[i].partClone != nullptr) {
				//unequip_biped_fn(bipedanim_sptr->get(), &bipedanim_sptr->get()->bufferedObjects[i], 1, 0, 0);
			}
		}
	}
}
uint64_t UnequipHook(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5)
{
	std::lock_guard<std::recursive_mutex> lock(g_bipedstate_mutex);
	RE::TESBoundObject* item = (RE::TESBoundObject*)arg3;
	bool done = false;
	uint64_t ret_code = real_unequip_fn(arg1, arg2, arg3, arg4, arg5);
	//auto biped_dtor_3d = (void (*)(RE::BipedAnim *, uint64_t, uint64_t))(REL::Offset(0x212460).address());
	auto biped_clear_3d = (void (*)(RE::BipedAnim*, uint64_t, uint64_t))(REL::Offset(unequip_all_offset).address());
	if (item && item->IsArmor()) {
		if (RE::TESObjectARMO* armor = item->As<RE::TESObjectARMO>()) {
			if (RE::Actor* actor = (RE::Actor*)arg2) {
				if (actor->IsDisabled()) {
					return ret_code;
				}
				if (actor->formType == RE::FormType::ActorCharacter) {
					if (actor->GetSkin() == armor) {
						return ret_code;
					}
					bool no1P = false;
					if (actor->GetBiped1(false) == actor->GetBiped1(true)) {
						no1P = true;
					}
					if (actor->GetBiped1(false)) {
						RE::BipedAnim* bipedanim = actor->GetBiped1(false).get();
						if (!BipedAnimToExtraWorn.contains(actor->GetBiped1(false).get())) {
							BipedAnimToExtraWorn.insert_or_assign(bipedanim, std::map<int, RE::BipedAnim*>());
						}
						bool found = false;
						if (BipedAnimToExtraWorn[actor->GetBiped1(false).get()].contains(item->formID & 0xFFFFFFFF)) {
							auto p = std::pair(item->formID & 0xFFFFFFFF, BipedAnimToExtraWorn[actor->GetBiped1(false).get()][item->formID & 0xFFFFFFFF]);

							if ((p.first & 0xFFFFFFFF) == (item->formID & 0xFFFFFFFF)) {
								if (p.second != nullptr) {
									biped_clear_3d(p.second, 1, 0);
								}

								done = true;
								found = true;
							}
						}
						//if (bipedanim->actorRef.get() != nullptr && bipedanim->actorRef.get().get() != nullptr && bipedanim->actorRef.get().get()->As<RE::Actor>() != nullptr) {

						//}
					}
					if (no1P == false) {
						if (actor->GetBiped1(true)) {
							RE::BipedAnim* bipedanim = actor->GetBiped1(true).get();
							if (!BipedAnimToExtraWorn.contains(actor->GetBiped1(true).get())) {
								BipedAnimToExtraWorn.insert_or_assign(bipedanim, std::map<int, RE::BipedAnim*>());
							}
							bool found = false;
							if (BipedAnimToExtraWorn[actor->GetBiped1(true).get()].contains(item->formID & 0xFFFFFFFF)) {
								auto p = std::pair(item->formID & 0xFFFFFFFF, BipedAnimToExtraWorn[actor->GetBiped1(true).get()][item->formID & 0xFFFFFFFF]);

								if ((p.first & 0xFFFFFFFF) == (item->formID & 0xFFFFFFFF)) {
									if (p.second != nullptr) {
										biped_clear_3d(p.second, 1, 0);
									}
									done = true;
									found = true;
								}
							}
						}
					}
					if (allow_unlimited == 0) {
						if (!ActorToVirtualSlotEquipment.contains(actor)) {
							ActorToVirtualSlotEquipment.insert(std::pair(actor, std::map<RE::FormID, RE::TESBoundObject*>()));
						}
						std::set<RE::FormID> ToRemove;
						for (auto& p : ActorToVirtualSlotEquipment[actor]) {
							if (VirtualSlots.contains(p.first)) {
								if (p.second->formID == item->formID) {
									ToRemove.insert(p.first);
								}
							}
						}
						for (auto templateId : ToRemove) {
							ActorToVirtualSlotEquipment[actor].erase(templateId);
						}
					}
				}
			}
		}
	}

	return ret_code;
}
void EquipArmorStuff(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4,
	uint64_t arg5)
{
	orig_equiparmorstuff(arg1, arg2, arg3, arg4, arg5);
}
void NewEquipBiped(RE::BipedAnim* bipedanim, uint64_t param_2, uint64_t param_3, uint64_t param_4, uint64_t param_5)
{
	std::lock_guard<std::recursive_mutex> lock(g_bipedstate_mutex);
	//unequip_mode = true;

	return orig_equipbiped_fn(bipedanim, param_2, param_3, param_4,
		param_5);
}
void fake_unequip(uint64_t arg1, RE::Actor* actor, RE::TESBoundObject* item, uint64_t arg4, uint64_t arg5, uint64_t arg6,
	uint64_t arg7, uint64_t arg8, uint64_t arg9, uint64_t arg10, uint64_t arg11)
{
	std::lock_guard<std::recursive_mutex> lock(g_bipedstate_mutex);
}

uint64_t NewAddWornItem(RE::Actor* actor, RE::TESBoundObject* item, int32_t count, uint64_t arg3, uint64_t arg4,
	uint64_t arg5)
{
	std::lock_guard<std::recursive_mutex> lock(g_bipedstate_mutex);

	if (!item || item->formType != RE::FormType::Armor) {
		return orig_addwornitem_fn(actor, item, count, arg3, arg4, arg5);
	}

	uint64_t retval = 0;

	bool no1P = false;
	bool hasExtraKeyword = false;
	if (actor) {
		uint64_t* actor_raw = (uint64_t*)actor;
		if ((actor_raw[biped_3p_offset / 8] != actor_raw[biped_1p_offset / 8]) && actor_raw[biped_3p_offset / 8] != 0 && actor_raw[biped_1p_offset / 8] != 0) {
			no1P = true;
		}
		if (allow_unlimited == 0) {
			if (!ActorToVirtualSlotEquipment.contains(actor)) {
				ActorToVirtualSlotEquipment.insert(std::pair(actor, std::map<RE::FormID, RE::TESBoundObject*>()));
			}

			if (RE::BGSKeywordForm* keywordForm = item->As<RE::BGSKeywordForm>()) {
				for (auto keyword : keywordForm->GetKeywords()) {
					if (VirtualSlots.contains(keyword->formID)) {
						hasExtraKeyword = true;
						if (ActorToVirtualSlotEquipment[actor].contains(keyword->formID)) {
							RE::TESBoundObject* object = ActorToVirtualSlotEquipment[actor][keyword->formID];
							ActorToVirtualSlotEquipment[actor].erase(keyword->formID);
							if (RE::TESObjectARMO* armor = object->As<RE::TESObjectARMO>()) {
								if (RE::ActorEquipManager::GetSingleton()->UnequipObject(actor, object, nullptr, 1, nullptr, false, false, true, true) == false) {
								}
							}
						}
					}
				}
			}

			if (hasExtraKeyword == false) {
				if (actor->GetBiped1(false)) {
					if (RE::TESObjectARMO* armor = item->As<RE::TESObjectARMO>()) {
						for (auto p : BipedAnimToExtraWorn[actor->GetBiped1(false).get()]) {
							if (RE::TESForm* ref = RE::TESForm::LookupByID(RE::FormID(p.first))) {
								if (RE::TESObjectARMO* extrawornarmor = ref->As<RE::TESObjectARMO>()) {
									bool skip_unequip = allow_unlimited == 1;

									for (auto k : extrawornarmor->GetKeywords()) {
										if (VirtualSlots.contains(k->formID)) {
											skip_unequip = true;
											break;
										}
									}
									if (skip_unequip == true) {
										continue;
									}
									if ((armor->bipedModelData.bipedObjectSlots.underlying() & extrawornarmor->bipedModelData.bipedObjectSlots.underlying()) != 0x0) {
										if (RE::ActorEquipManager::GetSingleton()->UnequipObject(actor, extrawornarmor, nullptr, 1, nullptr, false, false, true, true) == false) {
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (no1P == false) {
		if (actor->GetBiped1(true)) {
			RE::BipedAnim* bipedanim = actor->GetBiped1(true).get();
			if (!BipedAnimToExtraWorn.contains(actor->GetBiped1(true).get())) {
				BipedAnimToExtraWorn.insert_or_assign(bipedanim, std::map<int, RE::BipedAnim*>());
			}
			bool found = false;
			if (BipedAnimToExtraWorn[actor->GetBiped1(true).get()].contains(item->formID)) {
				found = true;
			}

			if (found == false) {
				RE::BipedAnim* (*construct_biped)(RE::BipedAnim*, RE::Actor*, RE::NiNode*) =
					(RE::BipedAnim * (*)(RE::BipedAnim*, RE::Actor*, RE::NiNode*))(REL::Offset(construct_biped_offset).address());
				RE::BipedAnim* new_biped = (RE::BipedAnim*)malloc(0x2778);
				construct_biped(new_biped, bipedanim->actorRef.get().get()->As<RE::Actor>(), bipedanim->root);
				new_biped->IncRef();
				new_biped->IncRef();
				BipedAnimToExtraWorn[bipedanim].insert(
					std::pair(item->formID & 0xFFFFFFFF, (RE::BipedAnim*)new_biped));
			}
			std::pair p(item->formID, BipedAnimToExtraWorn[bipedanim][item->formID]);
			{
				if ((p.first & 0xFFFFFFFF) == (item->formID & 0xFFFFFFFF)) {
					bool done2 = false;

					uint64_t* actor_raw = (uint64_t*)bipedanim->actorRef.get().get();
					if (actor_raw[biped_1p_offset / 8] != 0x0 && actor_raw[biped_1p_offset / 8] == (uint64_t)bipedanim) {
						if (item->IsArmor()) {
							if (actor->GetActorBase()) {
								actor->IncRefCount();
								item->IncRef();

								SKSE::GetTaskInterface()->AddTask([item, actor, actor_raw, bipedanim]() {
									if (actor) {
										if (!actor->IsDisabled()) {
											RE::BSTSmartPointer bipedptr((RE::BipedAnim*)actor_raw[biped_1p_offset / 8]);
											/* PrepareEquipBiped(
                                                (RE::TESObjectARMO *)item, actor->GetRace(), &bipedptr,
                                                (uint64_t)actor->GetActorBase()->actorData.actorBaseFlags.get() & 1);*/
										}
									}
									actor->DecRefCount();
									item->DecRef();
								});
							}
						}
					}
				}
			}
		}
	}
	if (actor->GetBiped1(false)) {
		RE::BipedAnim* bipedanim = actor->GetBiped1(false).get();
		if (!BipedAnimToExtraWorn.contains(actor->GetBiped1(false).get())) {
			BipedAnimToExtraWorn.insert_or_assign(bipedanim, std::map<int, RE::BipedAnim*>());
		}
		bool found = false;
		if (BipedAnimToExtraWorn[actor->GetBiped1(false).get()].contains(item->formID)) {
			found = true;
		}

		if (found == false) {
			RE::BipedAnim* (*construct_biped)(RE::BipedAnim*, RE::Actor*, RE::NiNode*) =
				(RE::BipedAnim * (*)(RE::BipedAnim*, RE::Actor*, RE::NiNode*))(REL::Offset(construct_biped_offset).address());
			RE::BipedAnim* new_biped = (RE::BipedAnim*)malloc(0x2778);
			construct_biped(new_biped, bipedanim->actorRef.get().get()->As<RE::Actor>(), bipedanim->root);
			new_biped->IncRef();
			new_biped->IncRef();
			BipedAnimToExtraWorn[bipedanim].insert(std::pair(item->formID & 0xFFFFFFFF, (RE::BipedAnim*)new_biped));
		}
		std::pair p(item->formID & 0xFFFFFFFF,
			BipedAnimToExtraWorn[actor->GetBiped1(false).get()][item->formID & 0xFFFFFFFF]);
		{
			if ((p.first & 0xFFFFFFFF) == (item->formID & 0xFFFFFFFF)) {
				uint64_t* actor_raw = (uint64_t*)bipedanim->actorRef.get().get();
				/* actor_raw[biped_3p_offset / 8] = (uint64_t)p.second;
                uint64_t Biped1P = actor_raw[biped_1p_offset / 8];
                if (no1P == false) {
                    if (BipedAnimToExtraWorn[actor->GetBiped1(true).get()].contains(item->formID & 0xFFFFFFFF)) {
                        actor_raw[biped_1p_offset / 8] = (uint64_t)BipedAnimToExtraWorn[actor->GetBiped1(true).get()][item->formID];
                    }
                }*/
				uint8_t* slotpatch_ptr = (uint8_t*)(REL::Offset(slotpatch_offset).address());
				
				if (allow_unlimited == 0) {
					if (hasExtraKeyword == false) {
						//slotpatch_ptr[0x0] = 0x0f;
						//slotpatch_ptr[0x1] = 0x84;
						retval |= orig_addwornitem_fn(actor, item, count, arg3, arg4, arg5);
					} else {
						uint8_t backup_call0[5];
						memcpy((void*)backup_call0, (uint8_t*)(REL::Offset(addwornitem_offset0).address()), 5);
						uint8_t backup_call1[5];
						memcpy((void*)backup_call1, (uint8_t*)(REL::Offset(addwornitem_offset1).address()), 5);
						REL::safe_fill((std::uintptr_t)(REL::Offset(addwornitem_offset0).address()), 0x90, 0x5);
						REL::safe_fill((std::uintptr_t)(REL::Offset(addwornitem_offset1).address()), 0x90, 0x5);
						retval |= orig_addwornitem_fn(actor, item, count, arg3, arg4, arg5);
						REL::safe_write((std::uintptr_t)(REL::Offset(addwornitem_offset0).address()), (const void*)backup_call0, 0x5);
						REL::safe_write((std::uintptr_t)(REL::Offset(addwornitem_offset1).address()), (const void*)backup_call1, 0x5);
					}
				} else {
					uint8_t backup_call0[5];
					memcpy((void*)backup_call0, (uint8_t*)(REL::Offset(addwornitem_offset0).address()), 5);
					uint8_t backup_call1[5];
					memcpy((void*)backup_call1, (uint8_t*)(REL::Offset(addwornitem_offset1).address()), 5);
					REL::safe_fill((std::uintptr_t)(REL::Offset(addwornitem_offset0).address()), 0x90, 0x5);
					REL::safe_fill((std::uintptr_t)(REL::Offset(addwornitem_offset1).address()), 0x90, 0x5);
					retval |= orig_addwornitem_fn(actor, item, count, arg3, arg4, arg5);
					REL::safe_write((std::uintptr_t)(REL::Offset(addwornitem_offset0).address()), (const void*)backup_call0, 0x5);
					REL::safe_write((std::uintptr_t)(REL::Offset(addwornitem_offset1).address()), (const void*)backup_call1, 0x5);
				}

				//slotpatch_ptr[0x0] = 0x48;
				//slotpatch_ptr[0x1] = 0xe9;
				if (allow_unlimited == 0) {
					if (hasExtraKeyword == false) {
						
					}
				}
				//if (retval & 0x1) {
				if (allow_unlimited == 0) {
					if (RE::BGSKeywordForm* keywordForm = item->As<RE::BGSKeywordForm>()) {
						for (auto keyword : keywordForm->GetKeywords()) {
							if (VirtualSlots.contains(keyword->formID)) {
								//item->IncRef();
								ActorToVirtualSlotEquipment[actor][keyword->formID] = item;
							}
						}
					}
				}
				auto actor_handle = actor->GetHandle();
				SKSE::GetTaskInterface()->AddTask([actor_handle]() {
					if (actor_handle.get() && actor_handle.get().get() != nullptr && actor_handle.get().get()->Is3DLoaded()) {
						Update3DHook(actor_handle.get().get());
					}
				});
				return retval;
				//} else {
				//if (RE::TESObjectARMO* armor = item->As<RE::TESObjectARMO>()) {
				//RE::ActorEquipManager::GetSingleton()->UnequipObject(actor, item, nullptr, 1, nullptr, false, false, true,false);
				//}
				//}

				
			}
		}
	}
	retval |= orig_addwornitem_fn(actor, item, count, arg3, arg4, arg5);
	auto actor_handle = actor->GetHandle();
	SKSE::GetTaskInterface()->AddTask([actor_handle]() {
		if (actor_handle.get() && actor_handle.get().get() != nullptr && actor_handle.get().get()->Is3DLoaded()) {
			Update3DHook(actor_handle.get().get());
		}
	});
	return retval;
}

void UnequipBipedHook(RE::BipedAnim* anim, RE::BIPOBJECT* obj, uint64_t arg3, uint64_t arg4, uint64_t arg5)
{
	std::lock_guard<std::recursive_mutex> lock(g_bipedstate_mutex);
	if (anim->actorRef.get() != nullptr && anim->actorRef.get().get() != nullptr && anim->actorRef.get().get()->As<RE::Actor>() != nullptr) {
		if (anim == anim->actorRef.get().get()->As<RE::Actor>()->GetBiped1(false).get()) {
			return unequip_biped_fn(anim, obj, arg3, arg4, arg5);
		}
		if (anim == anim->actorRef.get().get()->As<RE::Actor>()->GetBiped1(true).get()) {
			return unequip_biped_fn(anim, obj, arg3, arg4, arg5);
		}
		if (!ExtraWornSlotMasks.contains(anim->actorRef.get().get()->As<RE::Actor>())) {
			return unequip_biped_fn(anim, obj, arg3, arg4, arg5);
		}
		for (int slot = 0; slot < 0x2a; slot++) {
			if ((&anim->bufferedObjects[slot] == obj || &anim->objects[slot] == obj) && ExtraWornSlotMasks[anim->actorRef.get().get()->As<RE::Actor>()][slot] > 0) {
				RE::Actor* actor = anim->actorRef.get().get()->As<RE::Actor>();
				if (ExtraWornAddons.contains(actor)) {
					if (anim->bufferedObjects[slot].addon != nullptr) {
						if (ExtraWornAddons[actor].contains(anim->bufferedObjects[slot].addon->formID)) {
							ExtraWornAddons[actor].erase(anim->bufferedObjects[slot].addon->formID);
							for (uint32_t i = 0; i < 0x2a; i++) {
								if (((uint32_t)anim->bufferedObjects[slot].addon->GetSlotMask() & (1 << i)) != 0) {
									if (ExtraWornSlotMasks[anim->actorRef.get().get()->As<RE::Actor>()][i] > 0) {
										ExtraWornSlotMasks[anim->actorRef.get().get()->As<RE::Actor>()][i] -= 1;
									}
								}
							}
						}
					}
					if (anim->objects[slot].addon != nullptr) {
						if (ExtraWornAddons[actor].contains(anim->objects[slot].addon->formID)) {
							ExtraWornAddons[actor].erase(anim->objects[slot].addon->formID);
							for (uint32_t i = 0; i < 0x2a; i++) {
								if (((uint32_t)anim->objects[slot].addon->GetSlotMask()&(1<<i))!=0) {
									if (ExtraWornSlotMasks[anim->actorRef.get().get()->As<RE::Actor>()][i] > 0) {
										ExtraWornSlotMasks[anim->actorRef.get().get()->As<RE::Actor>()][i] -= 1;
									}
								}
							}
						}
					}
				}
				
			}
		}
	}
	return unequip_biped_fn(anim, obj, arg3, arg4, arg5);
}

void OnMessage(SKSE::MessagingInterface::Message* message)
{
	if (message->type == SKSE::MessagingInterface::kPreLoadGame) {
		std::lock_guard<std::recursive_mutex> lock(g_bipedstate_mutex);
#ifdef FOR1170
		auto biped_equip_finish = (void (*)(RE::BipedAnim*, uint64_t, uint64_t, uint64_t, uint64_t))(REL::Offset(equip_biped).address());
#else
		auto biped_equip_finish = (void (*)(RE::BipedAnim*, float, uint64_t))(REL::Offset(equip_biped).address());
#endif
		for (auto& p : BipedAnimToExtraWorn) {
			if (p.first != nullptr) {
				for (auto& ap : p.second) {
					if (ap.second != nullptr) {
						Clear3DHook(ap.second, 1, 0);
						if (ap.second->actorRef.get() != nullptr && ap.second->actorRef.get()->As<RE::Actor>() != nullptr && ap.second->actorRef.get()->As<RE::Actor>()->GetActorBase() != nullptr) {
							if (ap.second->root == nullptr) {
								ap.second->root = p.first->root;
							}
							biped_equip_finish(ap.second, ap.second->actorRef.get()->As<RE::Actor>()->GetActorBase()->IsFemale(), 0, 0, 0);
						}
					}
				}
			}
		}
		//BipedAnimToExtraWorn.clear();
		//ActorToVirtualSlotEquipment.clear();
		//ExtraWornSlotMasks.clear();
		//ExtraWornAddons.clear();
	}
}
/* void UpdateActorHook(RE::Actor* ref, float delta, uint64_t arg3, uint64_t arg4)
{
	
	void (*OriginalUpdate)(RE::TESObjectREFR*, float dt) = (void (*)(RE::TESObjectREFR* objRef, float dt))(OriginalUpdatePtr);
	OriginalUpdate(ref, delta);
	if (RE::Actor * actor=ref->As<RE::Actor>()) {
		if (actor->GetBiped1(false)) {
			if (BipedAnimToExtraWorn.contains(actor->GetBiped1(false).get())) {
				for (auto p : BipedAnimToExtraWorn[actor->GetBiped1(false).get()]) {
					p.second->root->UpdateDownwardPass
				}
			}
		}
	}
}*/
extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	InitializeLog();
	logger::info("Loaded plugin {} {}", Plugin::NAME, Plugin::VERSION.string());
	SKSE::Init(a_skse);
	SKSE::AllocTrampoline(512);
	logger::info("Loading armorunlimited.json");
	std::ifstream f("data\\skse\\plugins\\armorunlimited.json");
	json data = json::parse(f);
	json::number_unsigned_t allowoverride = data["allowunlimited"];
	allow_unlimited = (int)allowoverride;
	for (auto val : data["virtualslots"]) {
		json::number_unsigned_t keywordFormID = val;
		VirtualSlots.insert(RE::FormID(keywordFormID));
	}
	uint8_t* slotpatch_ptr = (uint8_t*)(REL::Offset(slotpatch_offset).address());
	DWORD oldProtect = 0;

	uintptr_t* AddWornItemVtable = (uintptr_t*)(REL::Offset(addwornitemvtable0).address());
	auto& trampoline = SKSE::GetTrampoline();
	/*
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
        */
	VirtualProtect((void*)slotpatch_ptr, 6, PAGE_EXECUTE_READWRITE, &oldProtect);
	//slotpatch_ptr[0x0] = 0x48;
	//slotpatch_ptr[0x1] = 0xe9;
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
	orig_unequip_all_fn = (void (*)(RE::BipedAnim*, uint64_t, uint64_t))(REL::Offset(unequip_all_offset).address());
	trampoline.write_branch<5>((REL::Offset(ret_from_bipedanim_dtor_offset + 1).address()), HookAfterBipedDtor);

	trampoline.write_call<5>((REL::Offset(call_unequipall_from_dtor_offset).address()), UnequipAllBipedDtor);
	ret_from_bipedanim_dtor[0] = 0x90;
#endif
	bool hook_worked = true;
	orig_addwornitem_fn =
		(uint64_t(*)(RE::Actor*, RE::TESBoundObject*, int32_t, uint64_t, uint64_t, uint64_t))(REL::Offset(addwornitem).address());
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)orig_addwornitem_fn, &NewAddWornItem);
	hook_worked &= (DetourTransactionCommit() == NO_ERROR);
	orig_equiparmor =
		(uint64_t (*)(RE::TESActorBase* actor, uint64_t arg2, RE::BSTSmartPointer<RE::BipedAnim>* bipedanim_sptr, RE::TESObjectARMO**))(REL::Offset(EquipArmor_offset).address());
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)orig_equiparmor, &EquipArmorHook);
	hook_worked &= (DetourTransactionCommit() == NO_ERROR);
	orig_init_worn_armor_addon_fn =
		(void (*)(RE::TESObjectARMA* aa, RE::TESObjectARMO* armor, RE::BSTSmartPointer<RE::BipedAnim>* bipedanim_sptr,
			uint64_t param_4))(REL::Offset(init_worn_armor_addon).address());
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)orig_init_worn_armor_addon_fn, &InitWornArmorAddonHook);
	hook_worked &= (DetourTransactionCommit() == NO_ERROR);
	real_unequip_fn = (uint64_t(*)(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5))(REL::Offset(real_unequip).address());
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)real_unequip_fn, &UnequipHook);
	hook_worked &= (DetourTransactionCommit() == NO_ERROR);
	for (uint64_t offset : clear_3d_hooks) {
		trampoline.write_call<5>(REL::Offset(offset).address(), Clear3DHook);
	}
	unequip_biped_fn = (void (*)(RE::BipedAnim*, RE::BIPOBJECT*, uint64_t, uint64_t, uint64_t))(REL::Offset(unequip_biped).address());
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)unequip_biped_fn, &UnequipBipedHook);
	hook_worked &= (DetourTransactionCommit() == NO_ERROR);
	orig_update_3d_hook_fn=(bool(*)(RE::Actor*))REL::RelocationID(19316, 19743).address();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)orig_update_3d_hook_fn, &Update3DHook);
	hook_worked &= (DetourTransactionCommit() == NO_ERROR);
	SKSE::GetMessagingInterface()->RegisterListener(OnMessage);
	/* if (OriginalUpdatePtr == nullptr) {
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			OriginalUpdatePtr = (void*)REL::Offset(OriginalUpdateOffset).address();

			DetourAttach(&OriginalUpdatePtr, &UpdateActorHook);
			DetourTransactionCommit();
		}*/
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
