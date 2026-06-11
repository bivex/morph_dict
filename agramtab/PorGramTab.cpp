#include "PorGramTab.h"

const static std::string PortuguesePartOfSpeech[] =
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
	"PART"
};

const static std::string PortugueseGrammems[] =
{
	"sg", "pl", "masc", "fem", "neut",
	"nom", "acc", "dat", "gen",
	"pres", "past", "fut", "impf", "pfv",
	"ind", "sbjv", "impv",
	"p1", "p2", "p3",
	"inf", "ger", "ptcp",
	"comp", "sup", "pos"
};


CPorGramTab::CPorGramTab()
{
	assert(sizeof(PortugueseGrammems) / sizeof(std::string) == PORTUGUESE_GRAMMEMS_COUNT);
	assert(sizeof(PortuguesePartOfSpeech) / sizeof(std::string) == PORTUGUESE_PART_OF_SPEECH_COUNT);

	for (size_t i = 0; i < GetMaxGrmCount(); i++)
		GetLine(i) = NULL;
	m_Language = morphPortuguese;
}

CPorGramTab::~CPorGramTab()
{
	for (size_t i = 0; i < GetMaxGrmCount(); i++)
		if (GetLine(i) != NULL)
			delete GetLine(i);
}

void CPorGramTab::LoadFromRegistry()
{
	ReadFromFolder(GetDefaultPath());
}

part_of_speech_t CPorGramTab::GetPartOfSpeechesCount() const {
	return PORTUGUESE_PART_OF_SPEECH_COUNT;
}

const char* CPorGramTab::GetPartOfSpeechStr(part_of_speech_t i, NamingAlphabet na) const {
	return PortuguesePartOfSpeech[i].c_str();
}

const char* CPorGramTab::GetPartOfSpeechStrLong(part_of_speech_t i) const {
	return GetPartOfSpeechStr(i);
}

grammem_t CPorGramTab::GetGrammemsCount() const {
	return PORTUGUESE_GRAMMEMS_COUNT;
}

const char* CPorGramTab::GetGrammemStr(size_t i, NamingAlphabet na) const {
	return PortugueseGrammems[i].c_str();
}

size_t CPorGramTab::GetMaxGrmCount() const {
	return eMaxGrmCount;
}

bool CPorGramTab::GleicheGenderNumber(const char* gram_code1, const char* gram_code2) const
{
	size_t i1 = GramcodeToLineIndex(gram_code1);
	size_t i2 = GramcodeToLineIndex(gram_code2);
	if (i1 >= GetMaxGrmCount() || i2 >= GetMaxGrmCount()) return true;
	const CAgramtabLine* l1 = GetLine(i1);
	const CAgramtabLine* l2 = GetLine(i2);
	if (!l1 || !l2) return true;

	grammems_mask_t g1 = l1->m_Grammems;
	grammems_mask_t g2 = l2->m_Grammems;

	grammems_mask_t gender1 = g1 & ((1ULL << porMasc) | (1ULL << porFem) | (1ULL << porNeut));
	grammems_mask_t gender2 = g2 & ((1ULL << porMasc) | (1ULL << porFem) | (1ULL << porNeut));
	if (gender1 && gender2 && gender1 != gender2) return false;

	grammems_mask_t number1 = g1 & ((1ULL << porSg) | (1ULL << porPl));
	grammems_mask_t number2 = g2 & ((1ULL << porSg) | (1ULL << porPl));
	if (number1 && number2 && number1 != number2) return false;

	return true;
}

bool CPorGramTab::GleicheSubjectPredicate(const char* gram_code1, const char* gram_code2) const
{
	size_t i1 = GramcodeToLineIndex(gram_code1);
	size_t i2 = GramcodeToLineIndex(gram_code2);
	if (i1 >= GetMaxGrmCount() || i2 >= GetMaxGrmCount()) return false;
	const CAgramtabLine* l1 = GetLine(i1);
	const CAgramtabLine* l2 = GetLine(i2);
	if (!l1 || !l2) return false;

	grammems_mask_t g1 = l1->m_Grammems;
	grammems_mask_t g2 = l2->m_Grammems;

	grammems_mask_t person1 = g1 & ((1ULL << porP1) | (1ULL << porP2) | (1ULL << porP3));
	grammems_mask_t person2 = g2 & ((1ULL << porP1) | (1ULL << porP2) | (1ULL << porP3));
	if (person1 && person2 && person1 != person2) return false;

	grammems_mask_t number1 = g1 & ((1ULL << porSg) | (1ULL << porPl));
	grammems_mask_t number2 = g2 & ((1ULL << porSg) | (1ULL << porPl));
	if (number1 && number2 && number1 != number2) return false;

	return true;
}

bool CPorGramTab::IsStrongClauseRoot(const part_of_speech_mask_t poses) const
{
	return is_verb_form(poses);
}

bool CPorGramTab::IsMorphNoun(part_of_speech_mask_t poses) const
{
	return (poses & (1 << porNOUN)) != 0;
}

bool CPorGramTab::is_morph_adj(part_of_speech_mask_t poses) const
{
	return (poses & (1 << porADJ)) != 0;
}

bool CPorGramTab::is_morph_participle(part_of_speech_mask_t poses) const
{
	return false;
}

bool CPorGramTab::is_morph_pronoun(part_of_speech_mask_t poses) const
{
	return (poses & (1 << porPRON)) != 0;
}

bool CPorGramTab::is_morph_pronoun_adjective(part_of_speech_mask_t poses) const
{
	return (poses & (1 << porDET)) != 0;
}

bool CPorGramTab::is_left_noun_modifier(part_of_speech_mask_t poses, grammems_mask_t grammems) const
{
	return is_morph_adj(poses) || is_morph_pronoun_adjective(poses);
}

bool CPorGramTab::is_numeral(part_of_speech_mask_t poses) const
{
	return (poses & (1 << porNUM)) != 0;
}

bool CPorGramTab::is_verb_form(part_of_speech_mask_t poses) const
{
	return (poses & (1 << porVERB)) != 0;
}

bool CPorGramTab::is_infinitive(part_of_speech_mask_t poses) const
{
	return false;
}

bool CPorGramTab::is_morph_predk(part_of_speech_mask_t poses) const
{
	return false;
}

bool CPorGramTab::is_morph_adv(part_of_speech_mask_t poses) const
{
	return (poses & (1 << porADV)) != 0;
}

bool CPorGramTab::is_morph_personal_pronoun(part_of_speech_mask_t poses, grammems_mask_t grammems) const
{
	return (poses & (1 << porPRON)) != 0;
}

bool CPorGramTab::IsSimpleParticle(const std::string& lemma, part_of_speech_mask_t poses) const
{
	return false;
}

bool CPorGramTab::IsSynNoun(part_of_speech_mask_t poses, const std::string& lemma) const
{
	return false;
}

bool CPorGramTab::IsStandardParamAbbr(const char* WordStrUpper) const
{
	return false;
}

bool CPorGramTab::GleicheCase(const char* gram_code_noun, const char* gram_code_adj) const
{
	return false;
}

bool CPorGramTab::GleicheCaseNumber(const char* gram_code1, const char* gram_code2) const
{
	return false;
}

grammems_mask_t CPorGramTab::GleicheGenderNumberCase(const char* common_gram_code_noun, const char* gram_code_noun, const char* gram_code_adj) const
{
	return 0;
}

bool CPorGramTab::PartOfSpeechIsProductive(part_of_speech_t p) const {
	return p == porNOUN || p == porADJ || p == porVERB || p == porADV;
}
