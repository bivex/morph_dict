#pragma once

#include "agramtab.h"

enum LatinPartOfSpeechEnum
{
	latNOUN = 0,
	latADJ = 1,
	latVERB = 2,
	latADV = 3,
	latDET = 4,
	latPRON = 5,
	latPREP = 6,
	latCONJ = 7,
	latINT = 8,
	latNUM = 9,
	LATIN_PART_OF_SPEECH_COUNT = 10
};

enum LatinGrammemsEnum {
	LATIN_SINGULAR = 0,
	LATIN_PLURAL = 1,
	LATIN_MASCULINUM = 2,
	LATIN_FEMINUM = 3,
	LATIN_NEUTRUM = 4,
	LATIN_COMMON = 5,
	LATIN_NOMINATIVE = 6,
	LATIN_ACCUSATIVE = 7,
	LATIN_DATIVE = 8,
	LATIN_GENITIVE = 9,
	LATIN_ABLATIVE = 10,
	LATIN_VOCATIVE = 11,
	LATIN_LOCATIVE = 12,
	LATIN_PRESENT = 13,
	LATIN_PAST = 14,
	LATIN_FUTURE = 15,
	LATIN_IMPERFECTIVE = 16,
	LATIN_PERFECTIVE = 17,
	LATIN_INDICATIVE = 18,
	LATIN_SUBJUNCTIVE = 19,
	LATIN_IMPERATIVE = 20,
	LATIN_FIRST_PERSON = 21,
	LATIN_SECOND_PERSON = 22,
	LATIN_THIRD_PERSON = 23,
	LATIN_INFINITIVE = 24,
	LATIN_GERUND = 25,
	LATIN_PARTICIPLE = 26,
	LATIN_ACTIVE = 27,
	LATIN_PASSIVE = 28,
	LATIN_GRAMMEMS_COUNT = 29
};

enum LatinClauseTypeEnum {
	LAT_VERB_T = 0,
	LATIN_CLAUSE_TYPE_COUNT = 1
};

class CLatGramTab : public CAgramtab
{
	const static size_t eStartUp = 0x4141;
	const static size_t eEndUp = 0x5A5B;
	const static size_t eMaxGrmCount = eEndUp - eStartUp;
public:
	CAgramtabLine* Lines[eMaxGrmCount];
	CLatGramTab();
	~CLatGramTab();

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
		return "Software\\Dialing\\Lemmatizer\\Latin\\Sgramtab";
	}

	long GetClauseTypeByName(const char*) const override { assert(false); return 0; }
	const char* GetClauseNameByType(long type) const override { return "VERB_T"; }
	bool GleicheGenderNumber(const char* gram_code1, const char* gram_code2) const override;
	bool GleicheSubjectPredicate(const char* gram_code1, const char* gram_code2) const override;
	const size_t GetClauseTypesCount() const { return LATIN_CLAUSE_TYPE_COUNT; }

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
