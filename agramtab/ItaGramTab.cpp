#include "ItaGramTab.h"

const static std::string ItalianPartOfSpeech[] =
{
	"NOUN",
	"VERB",
	"ADJ",
	"ADV",
	"PRON",
	"PREP",
	"CONJ",
	"INT",
	"NUM",
	"PART",
	"DET",
	"PROPN"
};

const static std::string ItalianGrammems[] =
{
	"sg", "pl",
	"masc", "fem",
	"nom", "acc", "dat", "gen",
	"pres", "past", "fut", "impf", "pfv",
	"ind", "sbjv", "impv", "cond",
	"p1", "p2", "p3",
	"inf", "ger", "ptcp",
	"pos", "act", "pass"
};


CItaGramTab::CItaGramTab()
{
	assert(sizeof(ItalianGrammems) / sizeof(std::string) == ITALIAN_GRAMMEMS_COUNT);
	assert(sizeof(ItalianPartOfSpeech) / sizeof(std::string) == ITALIAN_PART_OF_SPEECH_COUNT);

	for (size_t i = 0; i < GetMaxGrmCount(); i++)
		GetLine(i) = NULL;
	m_Language = morphItalian;
}

CItaGramTab::~CItaGramTab()
{
	for (size_t i = 0; i < GetMaxGrmCount(); i++)
		if (GetLine(i) != NULL)
			delete GetLine(i);
}

void CItaGramTab::LoadFromRegistry()
{
	ReadFromFolder(GetDefaultPath());
}

part_of_speech_t CItaGramTab::GetPartOfSpeechesCount() const {
	return ITALIAN_PART_OF_SPEECH_COUNT;
}

const char* CItaGramTab::GetPartOfSpeechStr(part_of_speech_t i, NamingAlphabet na) const {
	return ItalianPartOfSpeech[i].c_str();
}

const char* CItaGramTab::GetPartOfSpeechStrLong(part_of_speech_t i) const {
	return GetPartOfSpeechStr(i);
}

grammem_t CItaGramTab::GetGrammemsCount() const {
	return ITALIAN_GRAMMEMS_COUNT;
}

const char* CItaGramTab::GetGrammemStr(size_t i, NamingAlphabet na) const {
	return ItalianGrammems[i].c_str();
}

size_t CItaGramTab::GetMaxGrmCount() const {
	return eMaxGrmCount;
}

bool CItaGramTab::GleicheGenderNumber(const char* gram_code1, const char* gram_code2) const
{
	size_t i1 = GramcodeToLineIndex(gram_code1);
	size_t i2 = GramcodeToLineIndex(gram_code2);
	if (i1 >= GetMaxGrmCount() || i2 >= GetMaxGrmCount()) return true;
	const CAgramtabLine* l1 = GetLine(i1);
	const CAgramtabLine* l2 = GetLine(i2);
	if (!l1 || !l2) return true;

	grammems_mask_t g1 = l1->m_Grammems;
	grammems_mask_t g2 = l2->m_Grammems;

	grammems_mask_t number1 = g1 & ((1ULL << itaSg) | (1ULL << itaPl));
	grammems_mask_t number2 = g2 & ((1ULL << itaSg) | (1ULL << itaPl));
	if (number1 && number2 && number1 != number2) return false;

	grammems_mask_t gender1 = g1 & ((1ULL << itaMasc) | (1ULL << itaFem));
	grammems_mask_t gender2 = g2 & ((1ULL << itaMasc) | (1ULL << itaFem));
	if (gender1 && gender2 && gender1 != gender2) return false;

	return true;
}

bool CItaGramTab::GleicheSubjectPredicate(const char* gram_code1, const char* gram_code2) const
{
	size_t i1 = GramcodeToLineIndex(gram_code1);
	size_t i2 = GramcodeToLineIndex(gram_code2);
	if (i1 >= GetMaxGrmCount() || i2 >= GetMaxGrmCount()) return false;
	const CAgramtabLine* l1 = GetLine(i1);
	const CAgramtabLine* l2 = GetLine(i2);
	if (!l1 || !l2) return false;

	grammems_mask_t g1 = l1->m_Grammems;
	grammems_mask_t g2 = l2->m_Grammems;

	grammems_mask_t person1 = g1 & ((1ULL << itaP1) | (1ULL << itaP2) | (1ULL << itaP3));
	grammems_mask_t person2 = g2 & ((1ULL << itaP1) | (1ULL << itaP2) | (1ULL << itaP3));
	if (person1 && person2 && person1 != person2) return false;

	grammems_mask_t number1 = g1 & ((1ULL << itaSg) | (1ULL << itaPl));
	grammems_mask_t number2 = g2 & ((1ULL << itaSg) | (1ULL << itaPl));
	if (number1 && number2 && number1 != number2) return false;

	return true;
}

bool CItaGramTab::IsStrongClauseRoot(const part_of_speech_mask_t poses) const
{
	return is_verb_form(poses);
}

bool CItaGramTab::IsMorphNoun(part_of_speech_mask_t poses) const
{
	return (poses & (1 << itaNOUN)) != 0;
}

bool CItaGramTab::is_morph_adj(part_of_speech_mask_t poses) const
{
	return (poses & (1 << itaADJ)) != 0;
}

bool CItaGramTab::is_morph_participle(part_of_speech_mask_t poses) const
{
	return false;
}

bool CItaGramTab::is_morph_pronoun(part_of_speech_mask_t poses) const
{
	return (poses & (1 << itaPRON)) != 0;
}

bool CItaGramTab::is_morph_pronoun_adjective(part_of_speech_mask_t poses) const
{
	return false;
}

bool CItaGramTab::is_left_noun_modifier(part_of_speech_mask_t poses, grammems_mask_t grammems) const
{
	return is_morph_adj(poses);
}

bool CItaGramTab::is_numeral(part_of_speech_mask_t poses) const
{
	return (poses & (1 << itaNUM)) != 0;
}

bool CItaGramTab::is_verb_form(part_of_speech_mask_t poses) const
{
	return (poses & (1 << itaVERB)) != 0;
}

bool CItaGramTab::is_infinitive(part_of_speech_mask_t poses) const
{
	return false;
}

bool CItaGramTab::is_morph_predk(part_of_speech_mask_t poses) const
{
	return false;
}

bool CItaGramTab::is_morph_adv(part_of_speech_mask_t poses) const
{
	return (poses & (1 << itaADV)) != 0;
}

bool CItaGramTab::is_morph_personal_pronoun(part_of_speech_mask_t poses, grammems_mask_t grammems) const
{
	return (poses & (1 << itaPRON)) != 0;
}

bool CItaGramTab::IsSimpleParticle(const std::string& lemma, part_of_speech_mask_t poses) const
{
	return false;
}

bool CItaGramTab::IsSynNoun(part_of_speech_mask_t poses, const std::string& lemma) const
{
	return false;
}

bool CItaGramTab::IsStandardParamAbbr(const char* WordStrUpper) const
{
	return false;
}

bool CItaGramTab::GleicheCase(const char* gram_code_noun, const char* gram_code_adj) const
{
	return false;
}

bool CItaGramTab::GleicheCaseNumber(const char* gram_code1, const char* gram_code2) const
{
	return false;
}

grammems_mask_t CItaGramTab::GleicheGenderNumberCase(const char* common_gram_code_noun, const char* gram_code_noun, const char* gram_code_adj) const
{
	return 0;
}

bool CItaGramTab::PartOfSpeechIsProductive(part_of_speech_t p) const {
	return p == itaNOUN || p == itaADJ || p == itaVERB || p == itaADV;
}
