#pragma once

#include "agramtab.h"

enum PolishPartOfSpeechEnum
{
	polNOUN = 0,
	polVERB = 1,
	polADJ = 2,
	polADV = 3,
	polPRON = 4,
	polPREP = 5,
	polCONJ = 6,
	polINT = 7,
	polNUM = 8,
	polPART = 9,
	polDET = 10,
	POLISH_PART_OF_SPEECH_COUNT = 11
};

enum PolishGrammemsEnum {
	polSg = 0,
	polPl = 1,
	// gender (singular)
	polMasc = 2,
	polFem = 3,
	polNeut = 4,
	// gender (plural) — Polish distinguishes virile (personal masculine) from non-virile
	polVir = 5,
	polNvir = 6,
	// cases
	polNom = 7,
	polGen = 8,
	polDat = 9,
	polAcc = 10,
	polIns = 11,
	polLoc = 12,
	polVoc = 13,
	// person
	polP1 = 14,
	polP2 = 15,
	polP3 = 16,
	// tense
	polPres = 17,
	polPast = 18,
	polFut = 19,
	// aspect
	polImpf = 20,
	polPfv = 21,
	// mood
	polInd = 22,
	polImp = 23,
	polCond = 24,
	// verb forms / voice
	polInf = 25,
	polPtcp = 26,
	polAct = 27,
	polPass = 28,
	// degree
	polPos = 29,
	polComp = 30,
	polSup = 31,
	// animacy (accusative syncretism)
	polAnim = 32,
	polNanim = 33,
	POLISH_GRAMMEMS_COUNT = 34
};

enum PolishClauseTypeEnum {
	POL_VERB_T = 0,
	POLISH_CLAUSE_TYPE_COUNT = 1
};

class CPolGramTab : public CAgramtab
{
	const static size_t eStartUp = 0x4141; // AA (uppercase)
	const static size_t eEndUp = 0x5A5B;  // ZZ + 1
	const static size_t eMaxGrmCount = eEndUp - eStartUp;
public:
	CAgramtabLine* Lines[eMaxGrmCount];
	CPolGramTab();
	~CPolGramTab();

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
		return "Software\\Dialing\\Lemmatizer\\Polish\\Fgramtab";
	}

	long GetClauseTypeByName(const char*) const override { assert(false); return 0; }
	const char* GetClauseNameByType(long type) const override { return "VERB_T"; }
	bool GleicheGenderNumber(const char* gram_code1, const char* gram_code2) const override;
	bool GleicheSubjectPredicate(const char* gram_code1, const char* gram_code2) const override;
	const size_t GetClauseTypesCount() const { return POLISH_CLAUSE_TYPE_COUNT; }

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
