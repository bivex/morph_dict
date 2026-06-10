#include "LatGramTab.h"

const static std::string LatinPartOfSpeech[] =
{
	"NOUN",
	"ADJ",
	"VERB",
	"ADV",
	"DET",
	"PRON",
	"PREP",
	"CONJ",
	"INT",
	"NUM"
};

const static std::string LatinGrammems[] =
{
	"sg", "pl", "masc", "fem", "neut", "com",
	"nom", "acc", "dat", "gen", "abl", "voc", "loc",
	"pres", "past", "fut", "impf", "pfv",
	"ind", "sbjv", "impv",
	"p1", "p2", "p3",
	"inf", "ger", "ptcp",
	"act", "pass"
};

CLatGramTab::CLatGramTab()
{
	assert(sizeof(LatinGrammems) / sizeof(std::string) == LATIN_GRAMMEMS_COUNT);
	assert(sizeof(LatinPartOfSpeech) / sizeof(std::string) == LATIN_PART_OF_SPEECH_COUNT);

	for (size_t i = 0; i < GetMaxGrmCount(); i++)
		GetLine(i) = NULL;
	m_Language = morphLatin;
}

CLatGramTab::~CLatGramTab()
{
	for (size_t i = 0; i < GetMaxGrmCount(); i++)
		if (GetLine(i) != NULL)
			delete GetLine(i);
}

void CLatGramTab::LoadFromRegistry()
{
	ReadFromFolder(GetDefaultPath());
}

part_of_speech_t CLatGramTab::GetPartOfSpeechesCount() const {
	return LATIN_PART_OF_SPEECH_COUNT;
}

const char* CLatGramTab::GetPartOfSpeechStr(part_of_speech_t i, NamingAlphabet na) const {
	return LatinPartOfSpeech[i].c_str();
}

const char* CLatGramTab::GetPartOfSpeechStrLong(part_of_speech_t i) const {
	return GetPartOfSpeechStr(i);
}

grammem_t CLatGramTab::GetGrammemsCount() const {
	return LATIN_GRAMMEMS_COUNT;
}

const char* CLatGramTab::GetGrammemStr(size_t i, NamingAlphabet na) const {
	return LatinGrammems[i].c_str();
}

size_t CLatGramTab::GetMaxGrmCount() const {
	return eMaxGrmCount;
}

bool CLatGramTab::GleicheGenderNumber(const char* gram_code1, const char* gram_code2) const
{
	size_t i1 = GramcodeToLineIndex(gram_code1);
	size_t i2 = GramcodeToLineIndex(gram_code2);
	if (i1 >= GetMaxGrmCount() || i2 >= GetMaxGrmCount()) return true;
	const CAgramtabLine* l1 = GetLine(i1);
	const CAgramtabLine* l2 = GetLine(i2);
	if (!l1 || !l2) return true;

	grammems_mask_t g1 = l1->m_Grammems;
	grammems_mask_t g2 = l2->m_Grammems;

	grammems_mask_t gender1 = g1 & ((1ULL << LATIN_MASCULINUM) | (1ULL << LATIN_FEMINUM) | (1ULL << LATIN_NEUTRUM) | (1ULL << LATIN_COMMON));
	grammems_mask_t gender2 = g2 & ((1ULL << LATIN_MASCULINUM) | (1ULL << LATIN_FEMINUM) | (1ULL << LATIN_NEUTRUM) | (1ULL << LATIN_COMMON));
	if (gender1 && gender2 && gender1 != gender2 && !(gender1 & (1ULL << LATIN_COMMON)) && !(gender2 & (1ULL << LATIN_COMMON))) return false;

	grammems_mask_t number1 = g1 & ((1ULL << LATIN_SINGULAR) | (1ULL << LATIN_PLURAL));
	grammems_mask_t number2 = g2 & ((1ULL << LATIN_SINGULAR) | (1ULL << LATIN_PLURAL));
	if (number1 && number2 && number1 != number2) return false;

	return true;
}

bool CLatGramTab::GleicheSubjectPredicate(const char* gram_code1, const char* gram_code2) const
{
	size_t i1 = GramcodeToLineIndex(gram_code1);
	size_t i2 = GramcodeToLineIndex(gram_code2);
	if (i1 >= GetMaxGrmCount() || i2 >= GetMaxGrmCount()) return false;
	const CAgramtabLine* l1 = GetLine(i1);
	const CAgramtabLine* l2 = GetLine(i2);
	if (!l1 || !l2) return false;

	grammems_mask_t g1 = l1->m_Grammems;
	grammems_mask_t g2 = l2->m_Grammems;

	grammems_mask_t person1 = g1 & ((1ULL << LATIN_FIRST_PERSON) | (1ULL << LATIN_SECOND_PERSON) | (1ULL << LATIN_THIRD_PERSON));
	grammems_mask_t person2 = g2 & ((1ULL << LATIN_FIRST_PERSON) | (1ULL << LATIN_SECOND_PERSON) | (1ULL << LATIN_THIRD_PERSON));
	if (person1 && person2 && person1 != person2) return false;

	grammems_mask_t number1 = g1 & ((1ULL << LATIN_SINGULAR) | (1ULL << LATIN_PLURAL));
	grammems_mask_t number2 = g2 & ((1ULL << LATIN_SINGULAR) | (1ULL << LATIN_PLURAL));
	if (number1 && number2 && number1 != number2) return false;

	return true;
}

bool CLatGramTab::IsStrongClauseRoot(const part_of_speech_mask_t poses) const
{
	return is_verb_form(poses);
}

bool CLatGramTab::IsMorphNoun(part_of_speech_mask_t poses) const
{
	return (poses & (1 << latNOUN)) != 0;
}

bool CLatGramTab::is_morph_adj(part_of_speech_mask_t poses) const
{
	return (poses & (1 << latADJ)) != 0;
}

bool CLatGramTab::is_morph_participle(part_of_speech_mask_t poses) const
{
	return (poses & (1 << latVERB)) != 0 && (poses & (1 << LATIN_PARTICIPLE)) != 0;
}

bool CLatGramTab::is_morph_pronoun(part_of_speech_mask_t poses) const
{
	return (poses & (1 << latPRON)) != 0;
}

bool CLatGramTab::is_morph_pronoun_adjective(part_of_speech_mask_t poses) const
{
	return (poses & (1 << latDET)) != 0;
}

bool CLatGramTab::is_left_noun_modifier(part_of_speech_mask_t poses, grammems_mask_t grammems) const
{
	return is_morph_adj(poses) || is_morph_pronoun_adjective(poses) || is_morph_pronoun(poses);
}

bool CLatGramTab::is_numeral(part_of_speech_mask_t poses) const
{
	return (poses & (1 << latNUM)) != 0;
}

bool CLatGramTab::is_verb_form(part_of_speech_mask_t poses) const
{
	return (poses & (1 << latVERB)) != 0;
}

bool CLatGramTab::is_infinitive(part_of_speech_mask_t poses) const
{
	return (poses & (1 << latVERB)) != 0 && (poses & (1 << LATIN_INFINITIVE)) != 0;
}

bool CLatGramTab::is_morph_predk(part_of_speech_mask_t poses) const
{
	return false;
}

bool CLatGramTab::is_morph_adv(part_of_speech_mask_t poses) const
{
	return (poses & (1 << latADV)) != 0;
}

bool CLatGramTab::is_morph_personal_pronoun(part_of_speech_mask_t poses, grammems_mask_t grammems) const
{
	return (poses & (1 << latPRON)) != 0;
}

bool CLatGramTab::IsSimpleParticle(const std::string& lemma, part_of_speech_mask_t poses) const
{
	return false;
}

bool CLatGramTab::IsSynNoun(part_of_speech_mask_t poses, const std::string& lemma) const
{
	return false;
}

bool CLatGramTab::IsStandardParamAbbr(const char* WordStrUpper) const
{
	return false;
}

bool CLatGramTab::GleicheCase(const char* gram_code_noun, const char* gram_code_adj) const
{
	size_t i1 = GramcodeToLineIndex(gram_code_noun);
	size_t i2 = GramcodeToLineIndex(gram_code_adj);
	if (i1 >= GetMaxGrmCount() || i2 >= GetMaxGrmCount()) return true;
	const CAgramtabLine* l1 = GetLine(i1);
	const CAgramtabLine* l2 = GetLine(i2);
	if (!l1 || !l2) return true;

	grammems_mask_t case1 = l1->m_Grammems & ((1ULL << LATIN_NOMINATIVE) | (1ULL << LATIN_ACCUSATIVE) | (1ULL << LATIN_DATIVE) | (1ULL << LATIN_GENITIVE) | (1ULL << LATIN_ABLATIVE) | (1ULL << LATIN_VOCATIVE) | (1ULL << LATIN_LOCATIVE));
	grammems_mask_t case2 = l2->m_Grammems & ((1ULL << LATIN_NOMINATIVE) | (1ULL << LATIN_ACCUSATIVE) | (1ULL << LATIN_DATIVE) | (1ULL << LATIN_GENITIVE) | (1ULL << LATIN_ABLATIVE) | (1ULL << LATIN_VOCATIVE) | (1ULL << LATIN_LOCATIVE));
	return !case1 || !case2 || case1 == case2;
}

bool CLatGramTab::GleicheCaseNumber(const char* gram_code1, const char* gram_code2) const
{
	size_t i1 = GramcodeToLineIndex(gram_code1);
	size_t i2 = GramcodeToLineIndex(gram_code2);
	if (i1 >= GetMaxGrmCount() || i2 >= GetMaxGrmCount()) return true;
	const CAgramtabLine* l1 = GetLine(i1);
	const CAgramtabLine* l2 = GetLine(i2);
	if (!l1 || !l2) return true;

	grammems_mask_t case1 = l1->m_Grammems & ((1ULL << LATIN_NOMINATIVE) | (1ULL << LATIN_ACCUSATIVE) | (1ULL << LATIN_DATIVE) | (1ULL << LATIN_GENITIVE) | (1ULL << LATIN_ABLATIVE) | (1ULL << LATIN_VOCATIVE) | (1ULL << LATIN_LOCATIVE));
	grammems_mask_t case2 = l2->m_Grammems & ((1ULL << LATIN_NOMINATIVE) | (1ULL << LATIN_ACCUSATIVE) | (1ULL << LATIN_DATIVE) | (1ULL << LATIN_GENITIVE) | (1ULL << LATIN_ABLATIVE) | (1ULL << LATIN_VOCATIVE) | (1ULL << LATIN_LOCATIVE));
	if (case1 && case2 && case1 != case2) return false;

	grammems_mask_t number1 = l1->m_Grammems & ((1ULL << LATIN_SINGULAR) | (1ULL << LATIN_PLURAL));
	grammems_mask_t number2 = l2->m_Grammems & ((1ULL << LATIN_SINGULAR) | (1ULL << LATIN_PLURAL));
	if (number1 && number2 && number1 != number2) return false;

	return true;
}

grammems_mask_t CLatGramTab::GleicheGenderNumberCase(const char* common_gram_code_noun, const char* gram_code_noun, const char* gram_code_adj) const
{
	if (!GleicheGenderNumber(gram_code_noun, gram_code_adj) || !GleicheCase(gram_code_noun, gram_code_adj))
		return 0;
	return (1ULL << LATIN_MASCULINUM) | (1ULL << LATIN_FEMINUM) | (1ULL << LATIN_NEUTRUM) | (1ULL << LATIN_COMMON) | (1ULL << LATIN_SINGULAR) | (1ULL << LATIN_PLURAL) | (1ULL << LATIN_NOMINATIVE) | (1ULL << LATIN_ACCUSATIVE) | (1ULL << LATIN_DATIVE) | (1ULL << LATIN_GENITIVE) | (1ULL << LATIN_ABLATIVE) | (1ULL << LATIN_VOCATIVE) | (1ULL << LATIN_LOCATIVE);
}

bool CLatGramTab::PartOfSpeechIsProductive(part_of_speech_t p) const {
	return p == latNOUN || p == latADJ || p == latVERB || p == latADV;
}
