#pragma once

#include "agramtab.h"

enum HungarianPartOfSpeechEnum
{
	hunNOUN = 0,
	hunVERB = 1,
	hunADJ = 2,
	hunADV = 3,
	hunPRON = 4,
	hunPREP = 5,
	hunCONJ = 6,
	hunINT = 7,
	hunNUM = 8,
	hunPART = 9,
	hunDET = 10,
	HUNGARIAN_PART_OF_SPEECH_COUNT = 11
};

enum HungarianGrammemsEnum {
	hunSg = 0,
	hunPl = 1,
	hunNom = 2,
	hunAcc = 3,
	hunDat = 4,
	hunGen = 5,
	hunIns = 6,
	hunCau = 7,
	hunTra = 8,
	hunEss = 9,
	hunIll = 10,
	hunIne = 11,
	hunEla = 12,
	hunSub = 13,
	hunDel = 14,
	hunAll = 15,
	hunAde = 16,
	hunAbl = 17,
	hunTer = 18,
	hunLat = 19,
	hunP1 = 20,
	hunP2 = 21,
	hunP3 = 22,
	hunPres = 23,
	hunPast = 24,
	hunFut = 25,
	hunCond = 26,
	hunImpv = 27,
	hunInd = 28,
	hunSbjv = 29,
	hunDef = 30,
	hunIndef = 31,
	hunInf = 32,
	hunPtcp = 33,
	hunPos = 34,
	hunComp = 35,
	hunSup = 36,
	HUNGARIAN_GRAMMEMS_COUNT = 37
};

enum HungarianClauseTypeEnum {
	HUN_VERB_T = 0,
	HUNGARIAN_CLAUSE_TYPE_COUNT = 1
};

class CHunGramTab : public CAgramtab
{
	const static size_t eStartUp = 0x4141; // AA (uppercase)
	const static size_t eEndUp = 0x5A5B;  // ZZ + 1
	const static size_t eMaxGrmCount = eEndUp - eStartUp;
public:
	CAgramtabLine* Lines[eMaxGrmCount];
	CHunGramTab();
	~CHunGramTab();

	void LoadFromRegistry() override;

	part_of_speech_t GetPartOfSpeechesCount() const override;
	const char* GetPartOfSpeechStr(part_of_speech_t i, NamingAlphabet na = naDefault) const override;
	const char* GetPartOfSpeechStrLong(part_of_speech_t i) const override;
	grammem_t GetGrammemsCount() const override;
	const char* GetGrammemStr(size_t i, NamingAlphabet na = naDefault) const override;
	size_t GetMaxGrmCount() const override;
	CAgramtabLine*& GetLine(size_t LineNo) override { return Lines[LineNo]; }
	const CAgramtabLine* GetLine(size_t LineNo) const override { return Lines[LineNo]; }
	size_t GramcodeToLineIndex(const char* s) const override {
		if (!s || !s[0] || !s[1]) return eMaxGrmCount;
		int idx = (unsigned char)s[0] * 0x100 + (unsigned char)s[1] - eStartUp;
		if (idx < 0 || idx >= (int)eMaxGrmCount) return eMaxGrmCount;
		return (size_t)idx;
	}
	std::string LineIndexToGramcode(uint16_t i) const override
	{
		i += eStartUp;
		char res[3];
		res[0] = (i >> 8);
		res[1] = (0xFF & i);
		res[2] = 0;
		return res;
	}

	const char* GetRegistryString() const
	{
		return "Software\\Dialing\\Lemmatizer\\Hungarian\\Fgramtab";
	}

	long GetClauseTypeByName(const char*) const override { assert(false); return 0; }
	const char* GetClauseNameByType(long type) const override { return "VERB_T"; }
	bool GleicheGenderNumber(const char* gram_code1, const char* gram_code2) const override;
	bool GleicheSubjectPredicate(const char* gram_code1, const char* gram_code2) const override;
	const size_t GetClauseTypesCount() const { return HUNGARIAN_CLAUSE_TYPE_COUNT; }

	bool IsStrongClauseRoot(const part_of_speech_mask_t poses) const override;
	bool IsMorphNoun(part_of_speech_mask_t poses) const override;
	bool is_morph_adj(part_of_speech_mask_t poses) const override;
	bool is_morph_participle(part_of_speech_mask_t poses) const override;
	bool is_morph_pronoun(part_of_speech_mask_t poses) const override;
	bool is_morph_pronoun_adjective(part_of_speech_mask_t poses) const override;
	bool is_left_noun_modifier(part_of_speech_mask_t poses, grammems_mask_t grammems) const override;
	bool is_numeral(part_of_speech_mask_t poses) const override;
	bool is_verb_form(part_of_speech_mask_t poses) const override;
	bool is_infinitive(part_of_speech_mask_t poses) const override;
	bool is_morph_predk(part_of_speech_mask_t poses) const override;
	bool is_morph_adv(part_of_speech_mask_t poses) const override;
	bool is_morph_personal_pronoun(part_of_speech_mask_t poses, grammems_mask_t grammems) const override;
	bool IsSimpleParticle(const std::string& lemma, part_of_speech_mask_t poses) const override;
	bool IsSynNoun(part_of_speech_mask_t poses, const std::string& lemma) const override;
	bool IsStandardParamAbbr(const char* WordStrUpper) const override;
	bool GleicheCase(const char* gram_code_noun, const char* gram_code_adj) const override;
	bool GleicheCaseNumber(const char* gram_code1, const char* gram_code2) const override;
	grammems_mask_t GleicheGenderNumberCase(const char* common_gram_code_noun, const char* gram_code_noun, const char* gram_code_adj) const override;
	bool PartOfSpeechIsProductive(part_of_speech_t) const override;
};
