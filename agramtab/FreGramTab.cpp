#include "FreGramTab.h"

const static std::string FrenchPartOfSpeech[] =
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
	"NUM",
	"ART",
	"PART"
};

const static std::string FrenchGrammems[] =
{
	"sg", "pl", "masc", "fem",
	"pres", "past", "fut", "impf", "pfv",
	"cond", "ind", "sbjv", "impv",
	"p1", "p2", "p3",
	"inf", "ger", "ptcp",
	"comp", "sup", "pos"
};


CFreGramTab::CFreGramTab()
{
	assert(sizeof(FrenchGrammems) / sizeof(std::string) == FRENCH_GRAMMEMS_COUNT);
	assert(sizeof(FrenchPartOfSpeech) / sizeof(std::string) == FRENCH_PART_OF_SPEECH_COUNT);

	for (size_t i = 0; i < GetMaxGrmCount(); i++)
		GetLine(i) = NULL;
	m_Language = morphFrench;
}

CFreGramTab::~CFreGramTab()
{
	for (size_t i = 0; i < GetMaxGrmCount(); i++)
		if (GetLine(i) != NULL)
			delete GetLine(i);
}

void CFreGramTab::LoadFromRegistry()
{
	ReadFromFolder(GetDefaultPath());
}

part_of_speech_t CFreGramTab::GetPartOfSpeechesCount() const {
	return FRENCH_PART_OF_SPEECH_COUNT;
}

const char* CFreGramTab::GetPartOfSpeechStr(part_of_speech_t i, NamingAlphabet na) const {
	return FrenchPartOfSpeech[i].c_str();
}

const char* CFreGramTab::GetPartOfSpeechStrLong(part_of_speech_t i) const {
	return GetPartOfSpeechStr(i);
}

grammem_t CFreGramTab::GetGrammemsCount() const {
	return FRENCH_GRAMMEMS_COUNT;
}

const char* CFreGramTab::GetGrammemStr(size_t i, NamingAlphabet na) const {
	return FrenchGrammems[i].c_str();
}

size_t CFreGramTab::GetMaxGrmCount() const {
	return eMaxGrmCount;
}

bool CFreGramTab::GleicheGenderNumber(const char* gram_code1, const char* gram_code2) const
{
	size_t i1 = GramcodeToLineIndex(gram_code1);
	size_t i2 = GramcodeToLineIndex(gram_code2);
	if (i1 >= GetMaxGrmCount() || i2 >= GetMaxGrmCount()) return true;
	const CAgramtabLine* l1 = GetLine(i1);
	const CAgramtabLine* l2 = GetLine(i2);
	if (!l1 || !l2) return true;

	grammems_mask_t g1 = l1->m_Grammems;
	grammems_mask_t g2 = l2->m_Grammems;

	grammems_mask_t gender1 = g1 & ((1ULL << freMasc) | (1ULL << freFem));
	grammems_mask_t gender2 = g2 & ((1ULL << freMasc) | (1ULL << freFem));
	if (gender1 && gender2 && gender1 != gender2) return false;

	grammems_mask_t number1 = g1 & ((1ULL << freSg) | (1ULL << frePl));
	grammems_mask_t number2 = g2 & ((1ULL << freSg) | (1ULL << frePl));
	if (number1 && number2 && number1 != number2) return false;

	return true;
}

bool CFreGramTab::GleicheSubjectPredicate(const char* gram_code1, const char* gram_code2) const
{
	size_t i1 = GramcodeToLineIndex(gram_code1);
	size_t i2 = GramcodeToLineIndex(gram_code2);
	if (i1 >= GetMaxGrmCount() || i2 >= GetMaxGrmCount()) return false;
	const CAgramtabLine* l1 = GetLine(i1);
	const CAgramtabLine* l2 = GetLine(i2);
	if (!l1 || !l2) return false;

	grammems_mask_t g1 = l1->m_Grammems;
	grammems_mask_t g2 = l2->m_Grammems;

	grammems_mask_t person1 = g1 & ((1ULL << freP1) | (1ULL << freP2) | (1ULL << freP3));
	grammems_mask_t person2 = g2 & ((1ULL << freP1) | (1ULL << freP2) | (1ULL << freP3));
	if (person1 && person2 && person1 != person2) return false;

	grammems_mask_t number1 = g1 & ((1ULL << freSg) | (1ULL << frePl));
	grammems_mask_t number2 = g2 & ((1ULL << freSg) | (1ULL << frePl));
	if (number1 && number2 && number1 != number2) return false;

	return true;
}

bool CFreGramTab::IsStrongClauseRoot(const part_of_speech_mask_t poses) const
{
	return is_verb_form(poses);
}

bool CFreGramTab::IsMorphNoun(part_of_speech_mask_t poses) const
{
	return (poses & (1 << freNOUN)) != 0;
}

bool CFreGramTab::is_morph_adj(part_of_speech_mask_t poses) const
{
	return (poses & (1 << freADJ)) != 0;
}

bool CFreGramTab::is_morph_participle(part_of_speech_mask_t poses) const
{
	return false;
}

bool CFreGramTab::is_morph_pronoun(part_of_speech_mask_t poses) const
{
	return (poses & (1 << frePRON)) != 0;
}

bool CFreGramTab::is_morph_pronoun_adjective(part_of_speech_mask_t poses) const
{
	return (poses & (1 << freDET)) != 0;
}

bool CFreGramTab::is_left_noun_modifier(part_of_speech_mask_t poses, grammems_mask_t grammems) const
{
	return is_morph_adj(poses) || is_morph_pronoun_adjective(poses);
}

bool CFreGramTab::is_numeral(part_of_speech_mask_t poses) const
{
	return (poses & (1 << freNUM)) != 0;
}

bool CFreGramTab::is_verb_form(part_of_speech_mask_t poses) const
{
	return (poses & (1 << freVERB)) != 0;
}

bool CFreGramTab::is_infinitive(part_of_speech_mask_t poses) const
{
	return false;
}

bool CFreGramTab::is_morph_predk(part_of_speech_mask_t poses) const
{
	return false;
}

bool CFreGramTab::is_morph_adv(part_of_speech_mask_t poses) const
{
	return (poses & (1 << freADV)) != 0;
}

bool CFreGramTab::is_morph_personal_pronoun(part_of_speech_mask_t poses, grammems_mask_t grammems) const
{
	return (poses & (1 << frePRON)) != 0;
}

bool CFreGramTab::IsSimpleParticle(const std::string& lemma, part_of_speech_mask_t poses) const
{
	return false;
}

bool CFreGramTab::IsSynNoun(part_of_speech_mask_t poses, const std::string& lemma) const
{
	return false;
}

bool CFreGramTab::IsStandardParamAbbr(const char* WordStrUpper) const
{
	return false;
}

bool CFreGramTab::GleicheCase(const char* gram_code_noun, const char* gram_code_adj) const
{
	return false;
}

bool CFreGramTab::GleicheCaseNumber(const char* gram_code1, const char* gram_code2) const
{
	return false;
}

grammems_mask_t CFreGramTab::GleicheGenderNumberCase(const char* common_gram_code_noun, const char* gram_code_noun, const char* gram_code_adj) const
{
	return 0;
}

bool CFreGramTab::PartOfSpeechIsProductive(part_of_speech_t p) const {
	return p == freNOUN || p == freADJ || p == freVERB || p == freADV;
}
