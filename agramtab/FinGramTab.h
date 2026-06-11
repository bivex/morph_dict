#pragma once

#include "agramtab.h"

enum FinnishPartOfSpeechEnum
{
	finNOUN = 0,
	finADJ = 1,
	finVERB = 2,
	finADV = 3,
	finPRON = 4,
	finPREP = 5,
	finCONJ = 6,
	finINT = 7,
	finNUM = 8,
	finPART = 9,
	finADP = 10,
	finPROPN = 11,
	FINNISH_PART_OF_SPEECH_COUNT = 12
};

enum FinnishGrammemsEnum {
	finSg = 0,
	finPl = 1,
	finNom = 2,
	finGen = 3,
	finPar = 4,
	finIne = 5,
	finEla = 6,
	finIll = 7,
	finAde = 8,
	finAbl = 9,
	finAll = 10,
	finEss = 11,
	finTra = 12,
	finIns = 13,
	finAbs = 14,
	finCom = 15,
	finPres = 16,
	finPast = 17,
	finImp = 18,
	finCond = 19,
	finPot = 20,
	finInd = 21,
	finP1 = 22,
	finP2 = 23,
	finP3 = 24,
	finInf = 25,
	finPtcp = 26,
	finAct = 27,
	finPass = 28,
	finPos = 29,
	finNeg = 30,
	finPrf = 31,
	FINNISH_GRAMMEMS_COUNT = 32
};

enum FinnishClauseTypeEnum {
	FIN_VERB_T = 0,
	FINNISH_CLAUSE_TYPE_COUNT = 1
};

class CFinGramTab : public CAgramtab
{
	const static size_t eStartUp = 0x4141; // AA (uppercase)
	const static size_t eEndUp = 0x5A5B;  // ZZ + 1
	const static size_t eMaxGrmCount = eEndUp - eStartUp;
public:
	CAgramtabLine* Lines[eMaxGrmCount];
	CFinGramTab();
	~CFinGramTab();

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
		return "Software\\Dialing\\Lemmatizer\\Finnish\\Fgramtab";
	}

	long GetClauseTypeByName(const char*) const override { assert(false); return 0; }
	const char* GetClauseNameByType(long type) const override { return "VERB_T"; }
	bool GleicheGenderNumber(const char* gram_code1, const char* gram_code2) const override;
	bool GleicheSubjectPredicate(const char* gram_code1, const char* gram_code2) const override;
	const size_t GetClauseTypesCount() const { return FINNISH_CLAUSE_TYPE_COUNT; }

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
