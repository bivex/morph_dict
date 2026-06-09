#pragma once

#include "agramtab.h"

enum SpanishPartOfSpeechEnum
{
	spaNOUN = 0,
	spaADJ = 1,
	spaVERB = 2,
	spaADV = 3,
	spaDET = 4,
	spaPRON = 5,
	spaPREP = 6,
	spaCONJ = 7,
	spaINT = 8,
	spaNUM = 9,
	SPANISH_PART_OF_SPEECH_COUNT = 10
};

enum SpanishGrammemsEnum {
	sgSingular = 0,
	sgPlural = 1,
	sgMasculinum = 2,
	sgFeminum = 3,
	sgNeutrum = 4,
	sgNominative = 5,
	sgAccusative = 6,
	sgDative = 7,
	sgGenitive = 8,
	sgVocative = 9,
	sgPresent = 10,
	sgPast = 11,
	sgFuture = 12,
	sgImperfective = 13,
	sgPerfective = 14,
	sgConditional = 15,
	sgIndicative = 16,
	sgSubjunctive = 17,
	sgImperative = 18,
	sgFirstPerson = 19,
	sgSecondPerson = 20,
	sgThirdPerson = 21,
	sgInfinitive = 22,
	sgGerund = 23,
	sgParticiple = 24,
	sgComparative = 25,
	sgSuperlativ = 26,
	sgPositive = 27,
	sgFormal = 28,
	sgInformal = 29,
	sgNegative = 30,
	sgPro = 31,
	SPANISH_GRAMMEMS_COUNT = 32
};

enum SpanishClauseTypeEnum {
	SPA_VERB_T = 0,
	SPANISH_CLAUSE_TYPE_COUNT = 1
};

class CSpaGramTab : public CAgramtab
{
	const static size_t eStartUp = 0x4141; // AA (uppercase)
	const static size_t eEndUp = 0x5A5B;  // ZZ + 1
	const static size_t eMaxGrmCount = eEndUp - eStartUp;
public:
	CAgramtabLine* Lines[eMaxGrmCount];
	CSpaGramTab();
	~CSpaGramTab();

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
		return "Software\\Dialing\\Lemmatizer\\Spanish\\Sgramtab";
	}

	long GetClauseTypeByName(const char*) const override { assert(false); return 0; }
	const char* GetClauseNameByType(long type) const override { return "VERB_T"; }
	bool GleicheGenderNumber(const char* gram_code1, const char* gram_code2) const override;
	bool GleicheSubjectPredicate(const char* gram_code1, const char* gram_code2) const override;
	const size_t GetClauseTypesCount() const { return SPANISH_CLAUSE_TYPE_COUNT; }

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
